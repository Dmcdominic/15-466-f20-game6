#include "GridLoader.hpp"
#include "data_path.hpp"
#include "read_write_chunk.hpp"

#include <fstream>
#include <iostream>

#include "Tree.hpp"
#include "Player.hpp"
#include "Barrel.hpp"
#include "Tree.hpp"
#include "Railing.hpp"
#include "Rock.hpp"
#include "Bridge.hpp"
#include "Button.hpp"
#include "Disposal.hpp"
#include "Pit.hpp"
#include "Turnstile.hpp"
#include "Overworld.hpp"
#include "Grass.hpp"
#include "Ramp.hpp"
#include "Animal.hpp"
#include "Cloud.hpp"


Grid* GridLoader::load_level(unsigned int grid_id, Scene *scene) {
    //remove drawables from current grid 
    scene->drawables.clear(); 

    std::vector< int > obj_ids; 
    std::vector< PackedGrid > packed_grids; 

    std::ifstream in(data_path("./levels.bin"), std::ios::binary); 

    read_chunk(in, "objs", &obj_ids); 
    read_chunk(in, "grid", &packed_grids); 

    //we are loading grid 0 
    PackedGrid packed_grid = packed_grids[grid_id]; 
    Grid *grid = new Grid(packed_grid.width, packed_grid.height, packed_grid.goal, 0);

    int river_counter = 0;
    OverworldNode* first_node = nullptr;


    Bridge *bridge = nullptr;
    //set the BG objects 
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for(unsigned int x = 0; x < packed_grid.width; x++) {
            switch(obj_ids[packed_grid.data_start + x + y * packed_grid.width]) {
                case 7: {
                    grid->cells.at(x).at(y).set_bg_tile(new Turnstile(scene));
                    break;
                }
                case 8:
                    river_counter++;
                    break;
                case 11: {
                    grid->cells.at(x).at(y).set_bg_tile(new Disposal(scene));
                    break; 
                }
                case 13:
	                  river_counter++;
	                  break;
                case 14: 
                    grid->cells.at(x).at(y).set_bg_tile(new Grass(scene));
                    break;
                case 15: {
                    grid->cells.at(x).at(y).set_bg_tile(new Pit(scene));
                    break;
                }
                case 17: {
                    //TODO: shape path models
                    OverworldPath* overworldPath = new OverworldPath(scene);
                    grid->cells.at(x).at(y).set_bg_tile(overworldPath);
                    break;
                }
                case 18: {
                    OverworldNode* overworldNode = new OverworldNode(scene);
                    if (first_node == nullptr) first_node = overworldNode;
                    grid->cells.at(x).at(y).set_bg_tile(overworldNode);
                    break;
                }
            }
        }
    }


    // Start from first_node and set all the level_indices
    uint8_t level_index = 1;
    OverworldTile* OTile = first_node;
    std::vector<glm::ivec2> displ_to_try = { glm::ivec2(1, 0), glm::ivec2(-1, 0), glm::ivec2(0, 1), glm::ivec2(0, -1) };
    while (OTile != nullptr) {
      OTile->visited = true;
      OverworldNode* this_node = dynamic_cast<OverworldNode*>(OTile);
      OverworldPath* this_path = dynamic_cast<OverworldPath*>(OTile);
      // Update the node or path's level_index/max_adjacent_level
      if (this_node != nullptr) {
        this_node->level_index = level_index;
        level_index++;
        if (this_node->accessible()) grid->highest_level_node = this_node;
      } else if (this_path != nullptr) {
        this_path->max_adjacent_lvl = level_index;
        // Set path to be faded if this isn't accessible
        if (!this_path->faded && !this_path->accessible()) {
          this_path->make_faded();
        }
      } else {
        throw std::runtime_error("OverworldTile was neither an OverworldNode nor an OverworldPath...");
      }
      glm::ivec2 pos = OTile->cell->pos;
      OTile = nullptr;
      // Find the next OTile by checking the 4 neighbors
      for (auto displ_iter = displ_to_try.begin(); displ_iter != displ_to_try.end(); displ_iter++) {
        glm::ivec2 new_pos = pos + *displ_iter;
        if (!grid->is_valid_pos(new_pos)) continue; // has to be a valid position
        OverworldTile* next_tile = dynamic_cast<OverworldTile*>(grid->cell_at(new_pos)->bgTile);
        if (next_tile != nullptr && !next_tile->visited) {
          OTile = next_tile;
          break;
        }
      }
    }


    // Instantiate and shape the river/bridges
    std::vector< River* > *river_tiles = new std::vector< River* >(river_counter);
    int inserted = 0;
    bool prev_is_land = true; // Used for orienting bridge tiles
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for (unsigned int x = 0; x < packed_grid.width; x++) {
            switch(obj_ids[packed_grid.data_start + x + y * packed_grid.width]) {
                case 8:{
                    bridge = new Bridge(scene);

                    (*river_tiles)[inserted] = bridge;
                    inserted++;
                    grid->cells.at(x).at(y).set_bg_tile(bridge);
                    if (!prev_is_land) bridge->rotate_90();
                    bridge->position_models();
                    prev_is_land = false;
                    break;
                }
                case 13: {
                    //shape the river depending on surrounding tiles
                    bool upper = (y <= (packed_grid.height - 2))
                                &&((obj_ids[packed_grid.data_start + x + (y+1) * packed_grid.width] == 13)
                                || (obj_ids[packed_grid.data_start + x + (y+1) * packed_grid.width] == 8));
                    bool lower = (y >= 1)
                                &&((obj_ids[packed_grid.data_start + x + (y-1) * packed_grid.width] == 13)
                                || (obj_ids[packed_grid.data_start + x + (y-1) * packed_grid.width] == 8));
                    bool right = (x <= (packed_grid.width - 2))
                                &&((obj_ids[packed_grid.data_start + (x+1) + y * packed_grid.width] == 13)
                                || (obj_ids[packed_grid.data_start + (x+1) + y * packed_grid.width] == 8));
                    bool left = (x >= 1)
                                &&((obj_ids[packed_grid.data_start + (x-1) + y * packed_grid.width] == 13)
                                || (obj_ids[packed_grid.data_start + (x-1) + y * packed_grid.width] == 8));
                    River *river = new River(scene, left, right, upper, lower);

                    (*river_tiles)[inserted] = river;
                    inserted++;
                    grid->cells.at(x).at(y).set_bg_tile(river);

                    // Check if there's a railing here that we should load.
                    if(obj_ids[packed_grid.data_start + packed_grid.width * packed_grid.height + x + y * packed_grid.width]==16){
                        Railing *railing = new Railing(scene, left, right, upper, lower);
                        railing->drawable->transform->rotation = river->drawable->transform->rotation;
                        grid->cells.at(x).at(y).set_fg_obj(railing);
                    }

                    prev_is_land = false;
                    break;
                }
                default:
                    prev_is_land = true;
                    break;
            }
        }
        prev_is_land = false;
    }

    for(size_t i = 0; i < river_tiles->size(); i++){
        (*river_tiles)[i]->tiles = new std::vector< River* >(river_counter-1);
        int inserted = 0;
        for(size_t j = 0; j < river_tiles->size(); j++) {
            if (i != j) {
                (*river_tiles->at(i)->tiles)[inserted] = river_tiles->at(j);
                inserted++;
            }
        }
    }

    // Instantiate buttons
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for (unsigned int x = 0; x < packed_grid.width; x++) {
            switch (obj_ids[packed_grid.data_start + x + y * packed_grid.width]) {
                case 9:{
                    grid->cells.at(x).at(y).set_bg_tile(new Button(scene, bridge));
                    break;
                }
            }
        }
    }

    //set the FG objects
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for(unsigned int x = 0; x < packed_grid.width; x++) {
            int obj_id = obj_ids[packed_grid.data_start + packed_grid.width * packed_grid.height + x + y * packed_grid.width];
            switch(obj_id) {
                case 1: 
                    grid->player = new Player(scene);
                    grid->cells.at(x).at(y).set_fg_obj(grid->player);
                    break; 
                case 2:  
                    grid->cells.at(x).at(y).set_fg_obj(new Barrel(scene));
                    break; 
                case 3:
                    grid->cells.at(x).at(y).set_fg_obj(new Barrel(scene));
                    grid->cells.at(x).at(y).fgObj->rotate_90();
                    break; 
                case 4:  
                    grid->cells.at(x).at(y).set_fg_obj(new Rock(scene));
                    break;
                case 5:
                    grid->cells.at(x).at(y).set_fg_obj(new Tree(scene));
                    break;
                case 12:
                    grid->cells.at(x).at(y).set_fg_obj(new Animal(scene));
                    break;
                // 4 different colors for different ramp starting orientations
                case 19:
                case 20:
                case 21:
                case 22: {
                    Ramp* ramp = new Ramp(scene);
                    for (int rotations = obj_id - 19; rotations > 0; rotations--) {
                        ramp->rotate_90();
                    }
                    grid->cells.at(x).at(y).set_fg_obj(ramp);
                    break;
                }
            }
        }
    }


    //set the sky objects 
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for(unsigned int x = 0; x < packed_grid.width; x++) {
            switch(obj_ids[packed_grid.data_start + 2 * packed_grid.width * packed_grid.height + x + y * packed_grid.width]) {
                case 10: 
                    //TODO: cloud path 
                    grid->cells.at(x).at(y).set_sky_obj(new Cloud(scene));
                    break; 
            }
        }
    }


    
    return grid; 
}


// Returns a copy of src Grid, presumably current_grid, to be pushed onto the undo_grids stack.
Grid* GridLoader::create_undo_copy(Grid* src) {
  Grid* grid_cpy = new Grid(src->width, src->height, src->goal, src->num_disposed);
  
  // TODO
  return grid_cpy;
}


// Takes in an undo copy and fully loads it into the current_grid.
// FIRST properly delete and clean up the current_grid.
void GridLoader::load_undo_copy(Grid* undo_copy) {
  // TODO - properly load the new grid
  //        - Load models and add the new drawables to the drawables vector
  current_grid = new Grid(*undo_copy);
}

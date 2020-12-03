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
    unload_current_grid(scene);

    std::vector< int > obj_ids; 
    std::vector< PackedGrid > packed_grids; 

    std::ifstream in(data_path("./levels.bin"), std::ios::binary); 

    read_chunk(in, "objs", &obj_ids); 
    read_chunk(in, "grid", &packed_grids); 

    //we are loading grid 0 
    PackedGrid packed_grid = packed_grids[grid_id]; 
    Grid *grid = new Grid(packed_grid.width, packed_grid.height, packed_grid.goal, 0);

    OverworldNode* first_node = nullptr;


    Bridge *bridge = nullptr;
    //set the BG objects 
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for(unsigned int x = 0; x < packed_grid.width; x++) {
            Cell &cell = grid->cells.at(x).at(y);
            int obj_id = obj_ids[packed_grid.data_start + x + y * packed_grid.width];
            switch(obj_id) {
                case 7: {
                    cell.set_bg_tile(new Turnstile(scene));
                    break;
                }
                case 8:
                    break;
                case 11: {
                    cell.set_bg_tile(new Disposal(scene));
                    break; 
                }
                case 13:
	                  break;
                case 14: 
                    cell.set_bg_tile(new Grass(scene));
                    break;
                case 15: {
                    cell.set_bg_tile(new Pit(scene));
                    break;
                }
                case 17: {
                    bool upper = (y <= (packed_grid.height - 2))
                                &&((obj_ids[packed_grid.data_start + x + (y+1) * packed_grid.width] == 17)
                                || (obj_ids[packed_grid.data_start + x + (y+1) * packed_grid.width] == 18));
                    bool lower = (y >= 1)
                                &&((obj_ids[packed_grid.data_start + x + (y-1) * packed_grid.width] == 17)
                                || (obj_ids[packed_grid.data_start + x + (y-1) * packed_grid.width] == 18));
                    bool right = (x <= (packed_grid.width - 2))
                                &&((obj_ids[packed_grid.data_start + (x+1) + y * packed_grid.width] == 17)
                                || (obj_ids[packed_grid.data_start + (x+1) + y * packed_grid.width] == 18));
                    bool left = (x >= 1)
                                &&((obj_ids[packed_grid.data_start + (x-1) + y * packed_grid.width] == 17)
                                || (obj_ids[packed_grid.data_start + (x-1) + y * packed_grid.width] == 18));
                    OverworldPath* overworldPath = new OverworldPath(scene, left, right, upper, lower);
                    cell.set_bg_tile(overworldPath);
                    break;
                }
                case 18: {
                    OverworldNode* overworldNode = new OverworldNode(scene);
                    if (first_node == nullptr) first_node = overworldNode;
                    cell.set_bg_tile(overworldNode);
                    break;
                }
            }
            if (cell.bgTile != nullptr) cell.bgTile->load_and_reposition_models(scene);
        }
    }


    // Overworld - set the paths appropriately
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
        else {
            this_node->make_faded(); 
        }
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
    bool prev_is_land = true; // Used for orienting bridge tiles
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for (unsigned int x = 0; x < packed_grid.width; x++) {
            int obj_id = obj_ids[packed_grid.data_start + x + y * packed_grid.width];
            switch(obj_id) {
                case 8:{
                    bridge = new Bridge(scene, !prev_is_land);
                    grid->cells.at(x).at(y).set_bg_tile(bridge);
                    bridge->load_and_reposition_models(scene);
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

                    grid->cells.at(x).at(y).set_bg_tile(river);
                    river->load_and_reposition_models(scene);

                    // Check if there's a railing here that we should load.
                    if(obj_ids[packed_grid.data_start + packed_grid.width * packed_grid.height + x + y * packed_grid.width]==16){
                        Railing *railing = new Railing(scene, river->rotations, left, right, upper, lower);
                        grid->cells.at(x).at(y).set_fg_obj(railing);
                        railing->load_and_reposition_models(scene);
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


    //set the FG objects
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for(unsigned int x = 0; x < packed_grid.width; x++) {
            Cell& cell = grid->cells.at(x).at(y);
            int obj_id = obj_ids[packed_grid.data_start + packed_grid.width * packed_grid.height + x + y * packed_grid.width];
            switch(obj_id) {
                case 1: 
                    grid->player = new Player(scene);
                    cell.set_fg_obj(grid->player);
                    break; 
                case 2:  
                    cell.set_fg_obj(new Barrel(scene, 0));
                    break; 
                case 3:
                    cell.set_fg_obj(new Barrel(scene, 1));
                    break; 
                case 4:  
                    cell.set_fg_obj(new Rock(scene));
                    break;
                case 5:
                    cell.set_fg_obj(new Tree(scene));
                    break;
                case 12:
                    cell.set_fg_obj(new Animal(scene));
                    break;
                // 4 different colors for different ramp starting orientations
                case 19:
                case 20:
                case 21:
                case 22: {
                    int rotations = obj_id - 19;
                    Ramp* ramp = new Ramp(scene, rotations);
                    grid->cells.at(x).at(y).set_fg_obj(ramp);
                    break;
                }
            }
            if (cell.fgObj != nullptr) cell.fgObj->load_and_reposition_models(scene);
        }
    }


    // Instantiate buttons
    for (unsigned int y = 0; y < packed_grid.height; y++) {
      for (unsigned int x = 0; x < packed_grid.width; x++) {
        switch (obj_ids[packed_grid.data_start + x + y * packed_grid.width]) {
        case 9: {
          Button* button = new Button(scene, bridge->cell->pos);
          grid->cells.at(x).at(y).set_bg_tile(button);
          button->load_and_reposition_models(scene);
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
                    Cloud *cloud = new Cloud(scene);
                    grid->cells.at(x).at(y).set_sky_obj(cloud);
                    cloud->load_and_reposition_models(scene);
                    break;
            }
        }
    }

    
    return grid; 
}



// Clears the drawables and deletes the current_grid
void GridLoader::unload_current_grid(Scene *scene) {
  if (current_grid == nullptr) return;
  // clear all drawables. TODO - improve this so we don't clear ALL?
  scene->drawables.clear();
  delete current_grid;
  current_grid = nullptr;
}


// Returns a copy of src Grid, presumably current_grid, to be pushed onto the undo_grids stack.
Grid* GridLoader::create_undo_copy(Grid* src) {
  Grid* grid_cpy = new Grid(src->width, src->height, src->goal, src->num_disposed, src->grid_environment_score);

  for (size_t y = 0; y < grid_cpy->height; y++) {
    for (size_t x = 0; x < grid_cpy->width; x++) {
      glm::ivec2 pos = glm::ivec2(x, y);
      Cell* cell = grid_cpy->cell_at(pos);
      Cell* src_cell = src->cell_at(pos);
      // BgTile
      if (src_cell->bgTile != nullptr) {
        cell->bgTile = src_cell->bgTile->clone_lightweight(cell);
      }
      // FgObj
      if (src_cell->fgObj != nullptr) {
        cell->fgObj = src_cell->fgObj->clone_lightweight(cell);
      }
      // SkyObj
      if (src_cell->skyObj != nullptr) {
        cell->skyObj = src_cell->skyObj->clone_lightweight(cell);
      }
    }
  }

  grid_cpy->player = dynamic_cast<Player*>(grid_cpy->cell_at(src->player->cell->pos)->fgObj);
  if (grid_cpy->player == nullptr) throw std::runtime_error("Couldn't find player in create_undo_copy()");

  if (src->highest_level_node != nullptr) {
    grid_cpy->highest_level_node = dynamic_cast<OverworldNode*>(grid_cpy->cell_at(src->highest_level_node->cell->pos)->bgTile);
    if (grid_cpy->highest_level_node == nullptr) throw std::runtime_error("Couldn't find highest_level_node in create_undo_copy()");
  }

  return grid_cpy;
}


// Takes in an undo copy and fully loads it into the current_grid.
// FIRST properly delete and clean up the current_grid.
void GridLoader::load_undo_copy(Grid* undo_copy, Scene* scene) {
  if (current_grid != nullptr) throw std::runtime_error("Please delete current_grid before calling load_undo_copy()");
  current_grid = undo_copy;

  for (size_t y = 0; y < current_grid->height; y++) {
    for (size_t x = 0; x < current_grid->width; x++) {
      glm::ivec2 pos = glm::ivec2(x, y);
      Cell* cell = current_grid->cell_at(pos);
      // BgTile
      if (cell->bgTile != nullptr) {
        cell->bgTile->load_and_reposition_models(scene);
      }
      // FgObj
      if (cell->fgObj != nullptr) {
        cell->fgObj->load_and_reposition_models(scene);
        
      }
      // SkyObj
      if (cell->skyObj != nullptr) {
        cell->skyObj->load_and_reposition_models(scene);
      }
    }
  }
}

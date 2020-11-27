#include "GridLoader.hpp"
#include "data_path.hpp"
#include "read_write_chunk.hpp"

#include <fstream>

#include "Tree.hpp"
#include "Player.hpp"
#include "Barrel.hpp"
#include "Tree.hpp"
#include "Railing.hpp"
#include "Rock.hpp"
#include "Protesters.hpp"
#include "Bridge.hpp"
#include "Button.hpp"
#include "Disposal.hpp"
#include "Pit.hpp"
#include "Turnstile.hpp"
#include "Overworld.hpp"
#include "Ramp.hpp"


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
    Grid *grid = new Grid(packed_grid.width, packed_grid.height);
    grid->goal = packed_grid.goal;
    grid->num_disposed = 0;

    int river_counter = 0;
    OverworldNode* first_node = nullptr;


    Bridge *bridge = nullptr;
    //set the BG objects 
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for(unsigned int x = 0; x < packed_grid.width; x++) {
            switch(obj_ids[packed_grid.data_start + x + y * packed_grid.width]) {
                case 7: {
                    scene->drawables.push_back(model_loader->create_model("Turnstile")); 
                    Scene::Drawable grass = model_loader->create_model("Grass"); 
                    Turnstile *turnstile = new Turnstile(&(scene->drawables.back()), grass);
                    grid->cells.at(x).at(y).set_bg_tile(turnstile);
                    scene->drawables.push_back(grass);
                    turnstile->position_models(); 
                    break;
                }
                case 8:
                    river_counter++;
                    break;
                case 11: {
                    scene->drawables.push_back(model_loader->create_model("Disposal")); 
                    Scene::Drawable grass = model_loader->create_model("Grass"); 
                    Disposal *disposal = new Disposal(&(scene->drawables.back()), grass, &scene->drawables); 
                    grid->cells.at(x).at(y).set_bg_tile(disposal);
                    scene->drawables.push_back(grass);
                    disposal->position_models(); 
                    break; 
                }
                case 13:
	                  river_counter++;
	                  break;
                case 14: 
                    scene->drawables.push_back(model_loader->create_model("Grass"));
                    grid->cells.at(x).at(y).set_bg_tile(new BgTile(&(scene->drawables.back())));
                    break;
                case 15: {
                    scene->drawables.push_back(model_loader->create_model("Pit"));
                    Scene::Drawable grass = model_loader->create_model("Grass"); 
                    Pit *pit = new Pit(&(scene->drawables.back()), grass); 
                    grid->cells.at(x).at(y).set_bg_tile(pit);
                    scene->drawables.push_back(grass);
                    pit->position_models(); 
                    break;
                }
                case 17: {
                  scene->drawables.push_back(model_loader->create_model("Path"));
                  Scene::Drawable grass = model_loader->create_model("Grass");
                  OverworldPath* overworldPath = new OverworldPath(&(scene->drawables.back()), grass);
                  grid->cells.at(x).at(y).set_bg_tile(overworldPath);
                  scene->drawables.push_back(grass);
                  overworldPath->position_models();
                  break;
                }
                case 18: {
                  scene->drawables.push_back(model_loader->create_model("Node"));
                  Scene::Drawable grass = model_loader->create_model("Grass");
                  OverworldNode* overworldNode = new OverworldNode(&(scene->drawables.back()), grass);
                  if (first_node == nullptr) first_node = overworldNode;
                  grid->cells.at(x).at(y).set_bg_tile(overworldNode);
                  scene->drawables.push_back(grass);
                  overworldNode->position_models();
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
          this_path->faded = true;
          Scene::Drawable path_faded = model_loader->create_model("Path_Faded");
          path_faded.transform->position = this_path->drawable->transform->position;
          delete this_path->drawable->transform;
          *this_path->drawable = path_faded;
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
                    scene->drawables.push_back(model_loader->create_model("Bridge_Unactivated"));
                    //Scene::Drawable water = model_loader->create_model("Water");
                    Scene::Drawable grass = model_loader->create_model("Grass");
                    bridge = new Bridge(&(scene->drawables.back()),
                                        model_loader->create_model("Bridge"),
                                        &scene->drawables);
                    //scene->drawables.push_back(water);
    
                    (*river_tiles)[inserted] = bridge;
                    inserted++;
                    grid->cells.at(x).at(y).set_bg_tile(bridge);
                    //bridge->set_position_model(&(scene->drawables.back()));
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
                    int num_rotations = 0;
                    if(!left && !right && !upper && !lower){
                        scene->drawables.push_back(model_loader->create_model("River_Single"));
                    }
                    else if(left && right && !upper && !lower){
                        scene->drawables.push_back(model_loader->create_model("River_Straight"));
                        num_rotations = 1; 
                    }
                    else if(!left && !right && upper && lower){
                        scene->drawables.push_back(model_loader->create_model("River_Straight"));
                    }
                    else if(!left && right && !upper && lower){
                        scene->drawables.push_back(model_loader->create_model("River_Bent"));
                    }                    
                    else if(!left && right && upper && !lower){
                        scene->drawables.push_back(model_loader->create_model("River_Bent"));
                        num_rotations = 3; 
                    }                   
                    else if(left && !right && upper && !lower){
                        scene->drawables.push_back(model_loader->create_model("River_Bent"));
                        num_rotations = 2; 
                    }
                    else if(left && !right && !upper && lower){
                        scene->drawables.push_back(model_loader->create_model("River_Bent"));
                        num_rotations = 1; 
                    }       
                    else if(!left && !right && upper && !lower){
                        scene->drawables.push_back(model_loader->create_model("River_End"));
                    }                    
                    else if(!left && right && !upper && !lower){
                        scene->drawables.push_back(model_loader->create_model("River_End"));
                        num_rotations = 1; 
                    }                   
                    else if(!left && !right && !upper && lower){
                        scene->drawables.push_back(model_loader->create_model("River_End"));
                        num_rotations = 2; 
                    }
                    else if(left && !right && !upper && !lower){
                        scene->drawables.push_back(model_loader->create_model("River_End"));
                        num_rotations = 3; 
                    }                  
                    else{
                        scene->drawables.push_back(model_loader->create_model("River_None"));
                    }
                    River *river = new River(&(scene->drawables.back()),
                                            &scene->drawables);

                    (*river_tiles)[inserted] = river;
                    inserted++;
                    grid->cells.at(x).at(y).set_bg_tile(river);
                    river->position_models();
                    for(int i=0; i <num_rotations; i++) river->rotate_90();

                    // Check if there's a railing here that we should load.
                    // TODO - load bent railing?
                    if(obj_ids[packed_grid.data_start + packed_grid.width * packed_grid.height + x + y * packed_grid.width]==16){
                        scene->drawables.push_back(model_loader->create_model("Railing_Straight"));
                        Railing *railing = new Railing(&(scene->drawables.back()));
                        grid->cells.at(x).at(y).set_fg_obj(railing);
                        for(int i=0; i <num_rotations; i++) railing->rotate_90();
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
                    scene->drawables.push_back(model_loader->create_model("Button"));
                    Scene::Drawable grass = model_loader->create_model("Grass"); 
                    Button *button = new Button(&(scene->drawables.back()),
                                                model_loader->create_model("Button_Pushed"),
                                                grass,
                                                bridge);
                    grid->cells.at(x).at(y).set_bg_tile(button);
                    scene->drawables.push_back(grass);
                    button->position_models(); 
                    break;
                }
            }
        }
    }

    //set the FG objects
    int tree_id = 0;
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for(unsigned int x = 0; x < packed_grid.width; x++) {
            int obj_id = obj_ids[packed_grid.data_start + packed_grid.width * packed_grid.height + x + y * packed_grid.width];
            switch(obj_id) {
                case 1: 
                    scene->drawables.push_back(model_loader->create_model("Player")); 
                    grid->player = new Player(&(scene->drawables.back()),
                    		model_loader->create_model("Player"),
                    		model_loader->create_model("Player_idle0"));
                    grid->cells.at(x).at(y).set_fg_obj(grid->player);
                    break; 
                case 2:  
                    scene->drawables.push_back(model_loader->create_model("Barrel")); 
                    grid->cells.at(x).at(y).set_fg_obj(new Barrel(&(scene->drawables.back())));
                    break; 
                case 3:
                    scene->drawables.push_back(model_loader->create_model("Barrel")); 
                    grid->cells.at(x).at(y).set_fg_obj(new Barrel(&(scene->drawables.back())));
                    grid->cells.at(x).at(y).fgObj->rotate_90();
                    break; 
                case 4:  
                    scene->drawables.push_back(model_loader->create_model("Rock")); 
                    grid->cells.at(x).at(y).set_fg_obj(new Rock(&(scene->drawables.back())));
                    break;
                case 5:
                    scene->drawables.push_back(model_loader->create_model("Tree"));
		            tree_id++;
                    grid->cells.at(x).at(y).set_fg_obj(new Tree(&(scene->drawables.back()),
                    		model_loader->create_model("Tree"),
                    		model_loader->create_model("Tree_Flower1"), model_loader->create_model("Tree_Flower2"), tree_id));
                    grid->tree_flower_states.push_back(0);
                    break;
                case 6:
                    scene->drawables.push_back(model_loader->create_model("Protesters"));
                    grid->cells.at(x).at(y).set_fg_obj(new Protesters(&(scene->drawables.back()),
                                                                model_loader->create_model("Protesters_Contaminated"), &scene->drawables));
                    break;
                case 11:
                    scene->drawables.push_back(model_loader->create_model("Disposal")); 
                    grid->cells.at(x).at(y).set_fg_obj(new Tree(&(scene->drawables.back())));
                    break; 
                case 12:
                    scene->drawables.push_back(model_loader->create_model("Animal")); 
                    grid->cells.at(x).at(y).set_fg_obj(new Rock(&(scene->drawables.back())));
                    break;
                // 4 different colors for different ramp starting orientations
                case 19:
                case 20:
                case 21:
                case 22: {
                    scene->drawables.push_back(model_loader->create_model("Ramp"));
                    Ramp* ramp = new Ramp(&(scene->drawables.back()));
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
                    scene->drawables.push_back(model_loader->create_model("Cloud")); 
                    grid->cells.at(x).at(y).set_sky_obj(new SkyObj(&(scene->drawables.back())));
                    break; 
            }
        }
    }


    
    return grid; 
}


// Returns a copy of current_grid, to be pushed onto the undo_grids stack.
Grid* GridLoader::create_undo_copy() {
  // TODO
  return nullptr;
}
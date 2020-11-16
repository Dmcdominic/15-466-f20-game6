#include "GridLoader.hpp"
#include "data_path.hpp"
#include "read_write_chunk.hpp"
#include <fstream>
#include "Tree.hpp"
#include "Player.hpp"
#include "Barrel.hpp"
#include "Tree.hpp"
//#include "RottenTree.hpp"
#include "Rock.hpp"
#include "Protesters.hpp"
#include "Bridge.hpp"
#include "Button.hpp"
#include "Disposal.hpp"
#include "Pit.hpp"
#include "Turnstile.hpp"


Grid* GridLoader::load_level(unsigned int grid_id, ModelLoader loader, Scene *scene) {

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


    Bridge *bridge = nullptr;
    //set the BG objects 
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for(unsigned int x = 0; x < packed_grid.width; x++) {
            switch(obj_ids[packed_grid.data_start + x + y * packed_grid.width]) {
                case 7: {
                    scene->drawables.push_back(loader.create_model("Turnstile")); 
                    Scene::Drawable grass = loader.create_model("Grass"); 
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
                    scene->drawables.push_back(loader.create_model("Disposal")); 
                    Scene::Drawable grass = loader.create_model("Grass"); 
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
                    scene->drawables.push_back(loader.create_model("Grass"));
                    grid->cells.at(x).at(y).set_bg_tile(new BgTile(&(scene->drawables.back())));
                    break;
                case 15: {
                    scene->drawables.push_back(loader.create_model("Pit"));
                    Scene::Drawable grass = loader.create_model("Grass"); 
                    Pit *pit = new Pit(&(scene->drawables.back()), grass); 
                    grid->cells.at(x).at(y).set_bg_tile(pit);
                    scene->drawables.push_back(grass);
                    pit->position_models(); 
                    break;
                }
            }
        }
    }

    std::vector< River* > *river_tiles = new std::vector< River* >(river_counter);
    int inserted = 0;

    bool prev_is_land = true; // Used for orienting bridge tiles
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for (unsigned int x = 0; x < packed_grid.width; x++) {
            switch(obj_ids[packed_grid.data_start + x + y * packed_grid.width]) {
                case 8:{
                    scene->drawables.push_back(loader.create_model("Bridge_Unactivated"));
                    Scene::Drawable water = loader.create_model("Water");
                    bridge = new Bridge(&(scene->drawables.back()),
                                        loader.create_model("Bridge"),
                                        loader.create_model("Water_Toxic"),
                                        &scene->drawables);
                    scene->drawables.push_back(water);
    
                    (*river_tiles)[inserted] = bridge;
                    inserted++;
                    grid->cells.at(x).at(y).set_bg_tile(bridge);
                    bridge->set_position_model(&(scene->drawables.back()));
                    bridge->position_models();
                    if (prev_is_land) bridge->rotate_90();
                    prev_is_land = false;
                    break;
                }
                case 13: {
                    //TODO: shape the river depending on surrounding tiles
                    scene->drawables.push_back(loader.create_model("River_Straight"));
                    Scene::Drawable water = loader.create_model("Water");
                    River *river = new River(&(scene->drawables.back()),
                                             loader.create_model("Water_Toxic"),
                                             &scene->drawables);
                    scene->drawables.push_back(water);

                    (*river_tiles)[inserted] = river;
                    inserted++;
                    grid->cells.at(x).at(y).set_bg_tile(river);
                    river->set_position_model(&(scene->drawables.back()));
                    river->position_models();
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

    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for (unsigned int x = 0; x < packed_grid.width; x++) {
            switch (obj_ids[packed_grid.data_start + x + y * packed_grid.width]) {
                case 9:{
                    scene->drawables.push_back(loader.create_model("Button"));
                    Scene::Drawable grass = loader.create_model("Grass"); 
                    Button *button = new Button(&(scene->drawables.back()),
                                                loader.create_model("Button_Pushed"),
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
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for(unsigned int x = 0; x < packed_grid.width; x++) {
            switch(obj_ids[packed_grid.data_start + packed_grid.width * packed_grid.height + x + y * packed_grid.width]) {
                case 1: 
                    scene->drawables.push_back(loader.create_model("Player")); 
                    grid->player = new Player(&(scene->drawables.back()));
                    grid->cells.at(x).at(y).set_fg_obj(grid->player);
                    break; 
                case 2:  
                    scene->drawables.push_back(loader.create_model("Barrel")); 
                    grid->cells.at(x).at(y).set_fg_obj(new Barrel(&(scene->drawables.back())));
                    break; 
                case 3:
                    scene->drawables.push_back(loader.create_model("Barrel")); 
                    grid->cells.at(x).at(y).set_fg_obj(new Barrel(&(scene->drawables.back())));
                    grid->cells.at(x).at(y).fgObj->rotate_90();
                    break; 
                case 4:  
                    scene->drawables.push_back(loader.create_model("Rock")); 
                    grid->cells.at(x).at(y).set_fg_obj(new Rock(&(scene->drawables.back())));
                    break;
                case 5:
                    scene->drawables.push_back(loader.create_model("Tree"));
                    grid->cells.at(x).at(y).set_fg_obj(new Tree(&(scene->drawables.back())));
                    break;

                case 6:
                    scene->drawables.push_back(loader.create_model("Protesters"));
                    grid->cells.at(x).at(y).set_fg_obj(new Protesters(&(scene->drawables.back()),
                                                                loader.create_model("Protesters_Contaminated"), &scene->drawables));
                    break;
                case 11:
                    scene->drawables.push_back(loader.create_model("Disposal")); 
                    grid->cells.at(x).at(y).set_fg_obj(new Tree(&(scene->drawables.back())));
                    break; 
                case 12:
                    scene->drawables.push_back(loader.create_model("Animal")); 
                    grid->cells.at(x).at(y).set_fg_obj(new Rock(&(scene->drawables.back())));
                    break;            
            }
        }
    }

    //set the sky objects 
    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for(unsigned int x = 0; x < packed_grid.width; x++) {
            switch(obj_ids[packed_grid.data_start + 2 * packed_grid.width * packed_grid.height + x + y * packed_grid.width]) {
                case 10: 
                    //TODO: cloud path 
                    scene->drawables.push_back(loader.create_model("Cloud")); 
                    grid->cells.at(x).at(y).set_sky_obj(new SkyObj(&(scene->drawables.back())));
                    break; 
            }
        }
    }


    
    return grid; 
}

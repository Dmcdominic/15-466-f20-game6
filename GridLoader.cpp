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
                case 7:
                    scene->drawables.push_back(loader.create_model("Turnstile")); 
                    grid->cells.at(x).at(y).set_bg_tile(new Turnstile(&(scene->drawables.back())));
                    break;
                case 8:
                    river_counter++;
                    break;
                case 11:  
                    scene->drawables.push_back(loader.create_model("Disposal")); 
                    grid->cells.at(x).at(y).set_bg_tile(new Disposal(&(scene->drawables.back()), &scene->drawables));
                    break; 
                case 13:
	                  river_counter++;
	                  break;
                case 14:  
                    scene->drawables.push_back(loader.create_model("Grass"));
                    grid->cells.at(x).at(y).set_bg_tile(new BgTile(&(scene->drawables.back())));
                    break;
                case 15:
                  scene->drawables.push_back(loader.create_model("Pit"));
                  grid->cells.at(x).at(y).set_bg_tile(new Pit(&(scene->drawables.back())));
                  break;
            }
        }
    }

    std::vector< River* > *river_tiles = new std::vector< River* >(river_counter);
    int inserted = 0;

    for(unsigned int y = 0; y < packed_grid.height; y++) {
        for (unsigned int x = 0; x < packed_grid.width; x++) {
            switch(obj_ids[packed_grid.data_start + x + y * packed_grid.width]) {
                case 8:
                    scene->drawables.push_back(loader.create_model("River_Straight"));
                    bridge = new Bridge(&(scene->drawables.back()),
                                         loader.create_model("Tree"),
                                         loader.create_model("Bridge"),
                                         loader.create_model("Rock"),
                                         &scene->drawables);
                    (*river_tiles)[inserted] = bridge;
                    inserted++;
                    grid->cells.at(x).at(y).set_bg_tile(bridge);
                    break;
                case 13: {
                    //TODO: shape the river depending on surrounding tiles
                    scene->drawables.push_back(loader.create_model("River_Straight"));
                    River *river = new River(&(scene->drawables.back()),
                                             loader.create_model("River_Straight_Toxic"),
                                             &scene->drawables);
                    (*river_tiles)[inserted] = river;
                    inserted++;
                    grid->cells.at(x).at(y).set_bg_tile(river);
                    break;
                }
            }
        }
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
                case 9:
                    scene->drawables.push_back(loader.create_model("Button"));
                    grid->cells.at(x).at(y).set_bg_tile(new Button(&(scene->drawables.back()),
                                                                   loader.create_model("Button"),
                                                                   bridge));
                    break;
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

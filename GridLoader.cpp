#include "GridLoader.hpp"
#include "data_path.hpp"
#include "read_write_chunk.hpp"
#include <fstream>
#include "FixedRock.hpp"


Grid* GridLoader::load_level(unsigned int grid_id, ModelLoader loader, Scene *scene) {

    //remove drawables from current grid 
    scene->drawables.clear(); 

    std::vector< int > obj_ids; 
    std::vector< PackedGridLayer > layers; 

    std::ifstream in(data_path("../dist/levels.bin"), std::ios::binary); 

    read_chunk(in, "objs", &obj_ids); 
    read_chunk(in, "lyrs", &layers); 

    glm::uvec2 grid_size = glm::uvec2(30, 30); 

    //for now, we are assuming theres just one level in the bin file. 
    //TODO: variable level sizes
    Grid *grid = new Grid(grid_size.x, grid_size.y); 

    //TODO: find the correct PackedGridLayer

    for(unsigned int y = 0; y < grid_size.y; y++) {
        for(unsigned int x = 0; x < grid_size.x; x++) {
            //TODO: change which fgObj is placed in based on id 
            //TODO: repeat for bgObj and skyObj
            Scene::Drawable drawable = loader.create_model(obj_ids[x + y * 30] % 4); 
            grid->cells.at(x).at(y).set_fg_obj(new FixedRock(drawable.transform));
            scene->drawables.push_back(drawable); 

            std::cout << obj_ids[x + y * 30]; 
        }
        std::cout << "\n"; 
    }
    //TODO: repeat for all layers and difficulty levels
    
    return grid; 
}

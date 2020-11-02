#include "LevelLoader.hpp"
#include "data_path.hpp"
#include "read_write_chunk.hpp"
#include <fstream>

Level LevelLoader::load_level(unsigned int level_num) {

    std::vector< int > obj_ids; 
    std::vector< PackedGridLayer > layers; 

    std::ifstream in(data_path("../dist/levels.bin"), std::ios::binary); 

    read_chunk(in, "objs", &obj_ids); 
    read_chunk(in, "lyrs", &layers); 

    glm::uvec2 grid_size = glm::uvec2(30, 30); 

    //for now, we are assuming level 0 layer 0 difficulty 0. fix this later
    //TODO: variable level sizes
    Level level;
    Grid *easy = new Grid(grid_size.x, grid_size.y); 
    Grid *med = new Grid(grid_size.x, grid_size.y); 
    Grid *hard = new Grid(grid_size.x, grid_size.y); 

    //TODO: find the correct PackedGridLayer

    //TODO: actually put the data into a Level 
    for(unsigned int y = 0; y < grid_size.y; y++) {
        for(unsigned int x = 0; x < grid_size.x; x++) {
            //easyGrid.cells[row][col].fgObj = fgObj(); 
            std::cout << obj_ids[x + y * 30]; 
        }
        std::cout << "\n"; 
    }
    //TODO: repeat for all layers and difficulty levels
    level.easyGrid = easy; 
    level.medGrid = med; 
    level.hardGrid = med;

    return level; 
}

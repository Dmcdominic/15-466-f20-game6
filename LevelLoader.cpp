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

    //for now, we are assuming level 0 layer 0 difficulty 0. fix this later
    //TODO: variable level sizes
    Level level;     
    level.easyGrid = &Grid(30, 30); 
    level.medGrid = &Grid(30, 30); 
    level.hardGrid = &Grid(30, 30);

    //TODO: find the correct PackedGridLayer

    //TODO: actually put the data into a Level 
    for(unsigned int row = 0; row < 30; row++) {
        for(unsigned int col = 0; col < 30; col++) {
            //easyGrid.cells[row][col].fgObj = fgObj(); 
            std::cout << obj_ids[col + row * 30]; 
        }
    }
    //TODO: repeat for all layers and difficulty levels

    return level; 

}

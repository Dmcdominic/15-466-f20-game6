// based on https://github.com/lassyla/game1/blob/master/pack_tiles.cpp
// places level info into binary file
#include "load_save_png.hpp"
#include "read_write_chunk.hpp"
#include "data_path.hpp"
#include "PackedGridLayer.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <fstream>
#include <cstring>

int main(int argc, char **argv) {
    uint8_t num_levels = 1; 
    //files to read
	std::string level_filename = "../levels/level";

    std::vector< PackedGridLayer > layers; 
    std::vector< int > obj_ids; 
    std::vector< glm::u8vec4 > data; 

	uint8_t currentTile = 0; 

    //randomly chose 30x30 size for each grid 
    //TODO: variable level sizes 
    glm::uvec2 level_size = glm::uvec2(30, 30); 


    //load all level pngs
    //TODO: repeat for each layer and difficulty level
    for(int i = 0; i < num_levels; i++) {
        // std::vector <int> level(level_size.y * level_size.x); 

        std::cout << "loading " << (level_filename + std::to_string(i) + ".png") << "\n"; 
        load_png(data_path(level_filename  + std::to_string(i) + ".png"), &level_size, &data, LowerLeftOrigin); 
        std::cout << "\nsuccessfully loaded " << (level_filename + std::to_string(i) + ".png") << "\n"; 
        PackedGridLayer layer; 
        layer.level_id = i; 
        layer.difficulty = 0; 
        layer.layer_number = 0; 
        layer.data_start = (unsigned int) obj_ids.size(); 
        layer.width = level_size.x; 
        layer.height = level_size.y; 
        for(unsigned int y = 0; y < level_size.y; y++) {
            for(unsigned int x = 0; x < level_size.x; x++) {
                glm::u8vec4 color = data[x + y * level_size.x];
                // level[x + y * level_size.x] = int(color[0] / 50) + 5 * int(color[1] / 50);
                int id = int(color[0] / 50) + 5 * int(color[1] / 50);
                obj_ids.push_back(id);
                std::cout << id << " "; 
            }
            std::cout << "\n"; 
        }
        layers.push_back(layer); 
        // obj_ids.insert(obj_ids.end(), level.begin(), level.end());
    }

	std::ofstream out(data_path("../dist/levels.bin"), std::ios::binary);
    write_chunk("objs", obj_ids, &out); 
    write_chunk("lyrs", layers, &out); 
    std::cout << "\nsuccessfully saved levels "; 

	return 0;
}

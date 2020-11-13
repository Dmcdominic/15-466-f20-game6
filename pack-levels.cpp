// based on https://github.com/lassyla/game1/blob/master/pack_tiles.cpp
// places level info into binary file
#include "load_save_png.hpp"
#include "read_write_chunk.hpp"
#include "data_path.hpp"
#include "PackedGrid.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <fstream>
#include <cstring>

int main(int argc, char **argv) {
    //prefix of files to read
	std::string level_filename = "../levels/level";
    
    uint8_t num_levels = 8; 
    std::vector < glm::uvec2 > grid_sizes = {glm::uvec2(4,4), glm::uvec2(4,4), glm::uvec2(4,4), glm::uvec2(4,4), glm::uvec2(4,4), glm::uvec2(8,8), glm::uvec2(4,6), glm::uvec2(8,8)}; 
    std::vector < unsigned int > goals = {1, 2, 1, 1, 1, 2, 1, 1}; 
    
    std::vector< PackedGrid > grids; 
    std::vector< int > obj_ids; 
    std::vector< glm::u8vec4 > data; 

    //load all level pngs
    for(int i = 0; i < num_levels; i++) {

        PackedGrid grid;

        grid.width = grid_sizes[i].x; 
        grid.height = grid_sizes[i].y; 
        grid.goal = goals[i]; 
        grid.data_start = (unsigned int) obj_ids.size(); 

        //save bg 
        load_png(data_path(level_filename  + std::to_string(i) + "_bg.png"), &grid_sizes[i], &data, LowerLeftOrigin); 

        for(unsigned int y = 0; y < grid.height; y++) {
            for(unsigned int x = 0; x < grid.width; x++) {
                glm::u8vec4 color = data[x + y * grid.width];
                int id = int(color[0] / 50) + 6 * int(color[1] / 50);
                obj_ids.push_back(id);
            }
        }

        //save fg
        load_png(data_path(level_filename  + std::to_string(i) + "_fg.png"), &grid_sizes[i], &data, LowerLeftOrigin); 

        for(unsigned int y = 0; y < grid.height; y++) {
            for(unsigned int x = 0; x < grid.width; x++) {
                glm::u8vec4 color = data[x + y * grid.width];
                int id = int(color[0] / 50) + 6 * int(color[1] / 50);
                obj_ids.push_back(id);   
            }
        }

        //save sky
        load_png(data_path(level_filename  + std::to_string(i) + "_sky.png"), &grid_sizes[i], &data, LowerLeftOrigin); 

        for(unsigned int y = 0; y < grid.height; y++) {
            for(unsigned int x = 0; x < grid.width; x++) {
                glm::u8vec4 color = data[x + y * grid.width];
                int id = int(color[0] / 50) + 6 * int(color[1] / 50);
                obj_ids.push_back(id);
            }
        }
        grids.push_back(grid); 
    }

	std::ofstream out(data_path("../dist/levels.bin"), std::ios::binary);
    write_chunk("objs", obj_ids, &out); 
    write_chunk("grid", grids, &out); 
    std::cout << "\nsuccessfully saved levels "; 

	return 0;
}

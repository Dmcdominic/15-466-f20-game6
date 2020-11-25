// based on https://github.com/lassyla/game1/blob/master/pack_tiles.cpp
// places level info into binary file
#include "load_save_png.hpp"
#include "read_write_chunk.hpp"
#include "data_path.hpp"
#include "PackedGrid.hpp"
#include "level_sequence.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <fstream>
#include <cstring>

int main(int argc, char **argv) {
    //prefix of files to read
	const std::string level_filename = "../levels/level";
    
    std::vector< PackedGrid > grids; 
    std::vector< int > obj_ids; 
    std::vector< glm::u8vec4 > data; 

    //assert(level_sequence.size() == grid_sizes.size() && grid_sizes.size() == goals.size());

    //load all level pngs
    for(int i = 0; i < num_levels; i++) {
        std::string level_filename_full = level_filename + level_sequence[i].name;

        PackedGrid grid;

        grid.width = level_sequence[i].grid_size.x;
        grid.height = level_sequence[i].grid_size.y;
        grid.goal = level_sequence[i].goal;
        grid.data_start = (unsigned int) obj_ids.size(); 

        //save bg 
        load_png(data_path(level_filename_full + "_bg.png"), &level_sequence[i].grid_size, &data, LowerLeftOrigin);

        for(unsigned int y = 0; y < grid.height; y++) {
            for(unsigned int x = 0; x < grid.width; x++) {
                glm::u8vec4 color = data[x + y * grid.width];
                int id = int(color[0] / 50) + 6 * int(color[1] / 50);
                obj_ids.push_back(id);
            }
        }

        //save fg
        load_png(data_path(level_filename_full + "_fg.png"), &level_sequence[i].grid_size, &data, LowerLeftOrigin);

        for(unsigned int y = 0; y < grid.height; y++) {
            for(unsigned int x = 0; x < grid.width; x++) {
                glm::u8vec4 color = data[x + y * grid.width];
                int id = int(color[0] / 50) + 6 * int(color[1] / 50);
                obj_ids.push_back(id);   
            }
        }

        //save sky
        load_png(data_path(level_filename_full + "_sky.png"), &level_sequence[i].grid_size, &data, LowerLeftOrigin);

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

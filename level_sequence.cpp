#include "level_sequence.hpp"


std::vector<std::string> level_sequence = std::vector<std::string>({ "OW", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" });
std::vector < glm::uvec2 > grid_sizes = { glm::uvec2(20, 20), glm::uvec2(4,4), glm::uvec2(4,4), glm::uvec2(4,4), glm::uvec2(4,4), glm::uvec2(4,4), glm::uvec2(8,8), glm::uvec2(4,6), glm::uvec2(8,8), glm::uvec2(8,8), glm::uvec2(8,8) };
std::vector < unsigned int > goals = { 9, 1, 2, 1, 1, 1, 3, 1, 1, 1, 1 };

uint8_t num_levels = (uint8_t)level_sequence.size();

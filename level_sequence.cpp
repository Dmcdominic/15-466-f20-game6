#include "level_sequence.hpp"


std::vector<level> level_sequence = std::vector<level>({
    //level(name, grid_size, goal),
    level("OW", glm::uvec2(20, 20), 9),
    level("0", glm::uvec2(4, 4), 1),
    level("1", glm::uvec2(4, 4), 2),
    level("3", glm::uvec2(4, 4), 1),
    level("LongRocks", glm::uvec2(8, 8), 1),
    level("2", glm::uvec2(4, 4), 1),
    level("4", glm::uvec2(4, 4), 1),
    level("5", glm::uvec2(8, 8), 3),
    level("6", glm::uvec2(4,6), 1),
    level("7", glm::uvec2(8, 8), 1),
    level("8", glm::uvec2(8, 8), 1),
    level("9", glm::uvec2(8, 8), 1),
    level("Mush1", glm::uvec2(8, 8), 1),
    level("Ramp1", glm::uvec2(8, 8), 1),
    level("Ramp2", glm::uvec2(8, 8), 1),
    level("Ramp3", glm::uvec2(8, 8), 1),
  });


uint8_t num_levels = (uint8_t)level_sequence.size();

#include "level_sequence.hpp"


std::vector<level> level_sequence = std::vector<level>({
    //level(name, grid_size, goal),
    level("OW", glm::uvec2(20, 20), 9),
    //level("Test", glm::uvec2(13, 13), 1), // Includes one copy of each tile/object
    level("0", glm::uvec2(4, 4), 1),
    level("1", glm::uvec2(4, 4), 2),
    level("1b", glm::uvec2(7, 4), 2),
    level("Roll", glm::uvec2(6, 6), 4),
    level("RockTut", glm::uvec2(6, 5), 1),
    level("RockRev", glm::uvec2(7, 6), 1),
    level("RockNRoll", glm::uvec2(6, 7), 5),
    level("RockRiverTut", glm::uvec2(5, 5), 1),
    // TODO - rock river level(s)
    level("TurnstileTut", glm::uvec2(4, 4), 1),
    // TODO - turnstile levels (sequence of turnstiles in zig zag)
    level("ParityTutNoRock", glm::uvec2(5, 5), 1),
    // TODO - Parity (no rock) levels
    level("ParityTut", glm::uvec2(5, 5), 1),
    // TODO - Parity WITH rock levels
    level("LongRocks", glm::uvec2(8, 7), 1),
    level("2", glm::uvec2(4, 4), 1),
    level("4", glm::uvec2(4, 4), 1),
    level("5", glm::uvec2(8, 8), 3),
    level("6", glm::uvec2(4, 6), 1),
    level("7", glm::uvec2(8, 8), 1),
    level("AcornTut", glm::uvec2(5, 5), 1),
    level("8", glm::uvec2(8, 8), 1),
    level("9", glm::uvec2(8, 8), 1),
    level("Mush1", glm::uvec2(8, 8), 1),
    level("Ramp1", glm::uvec2(8, 8), 1),
    level("Ramp2", glm::uvec2(8, 8), 1),
    level("Ramp3", glm::uvec2(8, 8), 1),
    //level("T1", glm::uvec2(9, 9), 1),
    level("T3", glm::uvec2(8, 8), 1),
    level("T4", glm::uvec2(8, 8), 1),
    level("T2", glm::uvec2(8, 10), 1),
  });


uint8_t num_levels = (uint8_t)level_sequence.size();

#include "level_sequence.hpp"


std::vector<level> level_sequence = std::vector<level>({
    //level(name, grid_size, goal),
    level("OW", 9),
    //level("Test", 1), // Includes one copy of each tile/object
    level("0", 1),
    level("1", 2),
    level("1b", 2),
    level("Roll", 4),
    level("RockTut", 1),
    level("RockRev", 1),
    level("RockNRoll", 5),
    level("RockRiverTut", 1), // TODO - this could be simplified to make you try the rock -> river thing
    // TODO - rock river level(s)
    level("TurnstileTut", 1),
    level("TurnstileCircle", 1),
    level("ParityTutNoRock", 1),
    level("TurnstileScramble", 1),
    level("RiverTParity", 1),
    level("ParityTut", 1),
    level("LongRocks", 1), // TODO - Make this require rock parity (rather than no-rock parity)?
    level("2", 1),
    level("4", 1),
    level("5", 3),
    level("6", 1),
    level("7", 1),
    level("AcornTut", 1),
    level("8", 1),
    level("9", 1),
    level("Mush1", 1),
    level("Ramp1", 1),
    level("RockSnake", 1),
    level("Ramp1_5", 1),
    level("Ramp2", 1),
    level("Ramp2_5", 1),
    level("Ramp3", 1),
    //level("T1", 1),
    level("T3", 1),
    level("T4", 1),
    level("T2", 1),
  });


uint8_t num_levels = (uint8_t)level_sequence.size();

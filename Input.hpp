#pragma once

#include <optional>


// Types of input
enum class InputType {
  UP, DOWN, LEFT, RIGHT, INTERACT, UNDO, RESET, SKIP_LVL
};


// A struct passed to each cell in the grid whenever there's some user input.
struct Input {
  // Fields
  InputType type;

  // Constructor
  Input(InputType _type) : type(_type) {};
};



// Things that we might want to inform PlayMode after some input is/isn't handled.
struct Output {
  // Fields
  std::optional<uint8_t> level_to_load = std::nullopt; // If this is set, we will load the corresponding level
};
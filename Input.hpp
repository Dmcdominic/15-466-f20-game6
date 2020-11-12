#pragma once


enum class InputType {
  UP, DOWN, LEFT, RIGHT
};


struct Input {
  // Fields
  InputType type;

  // Constructor
  Input(InputType _type) : type(_type) {};
};
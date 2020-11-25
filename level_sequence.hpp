#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>


struct level {
  std::string name;
  glm::uvec2 grid_size;
  unsigned int goal;

  level(std::string _name, glm::uvec2 _grid_size, unsigned int _goal) : name(_name), grid_size(_grid_size), goal(_goal) {};
};


extern std::vector<level> level_sequence;

extern uint8_t num_levels;
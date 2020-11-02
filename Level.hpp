#pragma once
#include "Grid.hpp"

struct Level {
    unsigned int id; 
    unsigned int width; 
    unsigned int height; 
    Grid easyGrid; 
    Grid medGrid;
    Grid hardGrid; 
};

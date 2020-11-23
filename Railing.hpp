#pragma once

#include "Grid.hpp"
#include "Scene.hpp"


struct Railing : FgObj {
    // Constructors (inherited)
    using FgObj::FgObj;

    // Methods
    bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
};
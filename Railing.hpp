#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"


struct Railing : FgObj {
    // Fields
    bool left, right, upper, lower;

    // Constructor
    Railing(Scene *scene, int _rotations, bool left, bool right, bool upper, bool lower);

    // Methods
    std::string get_model_name() override { return "Railing_Straight"; }

    void load_models(Scene* scene) override;
    Railing* clone_lightweight(Cell* new_cell) override;

    bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
};
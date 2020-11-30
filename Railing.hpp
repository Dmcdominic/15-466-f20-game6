#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"


struct Railing : FgObj {
    // Constructors (inherited)
    using FgObj::FgObj;

    // Methods
    std::string get_model_name() override { return "Railing_Straight"; }

    void load_models(Scene* scene) override;
    Railing* clone_lightweight(Cell* new_cell) override;

    bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
};
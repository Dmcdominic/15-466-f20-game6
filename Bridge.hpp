#pragma once

#include "River.hpp"
#include "ModelLoader.hpp"
#include "Scene.hpp"


struct Bridge : River {
    // Fields
    Scene::Drawable* unactivated = nullptr;
    Scene::Drawable* bridge = nullptr;
    bool activated = false;

    // Constructors
    Bridge(Scene* scene, bool vertical);

    // Methods
    void load_models(Scene* scene) override;
    void position_models() override;
    Bridge* clone_lightweight(Cell* new_cell) override;

    bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
    void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
    void activate();
    void deactivate();

    void rotate_90(bool skip_incr = false) override;
};
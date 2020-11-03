#pragma once

#include "Grid.hpp"
#include "Scene.hpp"


struct Protesters : BgTile {
    // Constructors (inherited)

    // Fields
    static Scene::Drawable* prefab;
    Scene::Drawable* current = nullptr;
    Scene::Drawable rolled_over = nullptr;

    // Methods
    Protesters(Scene::Drawable* _drawable, Scene::Drawable _rotten): BgTile(_drawable->transform), current(_drawable), rolled_over(_rotten){};
//    void set_rotten_tree(Scene::Transform* _transform) : rotten(_transform) {};
    void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
};
#pragma once

#include "Grid.hpp"
#include "Scene.hpp"


struct Tree : BgTile {
    // Constructors (inherited)

    // Fields
    static Scene::Drawable* prefab;
    Scene::Drawable* current = nullptr;
    Scene::Drawable rotten = nullptr;

    // Methods
    Tree(Scene::Drawable* _drawable, Scene::Drawable _rotten): BgTile(_drawable->transform), current(_drawable), rotten(_rotten){};
    void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
};
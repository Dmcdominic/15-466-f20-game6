#pragma once

#include "Grid.hpp"
#include "RottenTree.hpp"
#include "Scene.hpp"


struct Tree : FgObj {
    // Constructors (inherited)

    // Fields
    static Scene::Drawable* prefab;
//    Scene::Drawable* current = nullptr;
    Scene::Drawable rotten;
    std::list<Scene::Drawable>* drawables = nullptr;

    // Methods
    Tree(Scene::Drawable* _drawable, Scene::Drawable _rotten, std::list<Scene::Drawable>* _drawables): FgObj(_drawable),
    rotten(_rotten), drawables(_drawables){};
    bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
    void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
};
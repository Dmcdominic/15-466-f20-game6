#pragma once

#include "Grid.hpp"
#include "Scene.hpp"


struct Protesters : FgObj {
    // Constructors (inherited)

    // Fields
    static Scene::Drawable* prefab;
    Scene::Drawable rolled_over = nullptr;
    std::list<Scene::Drawable>* drawables = nullptr;

    // Methods
    Protesters(Scene::Drawable* _drawable, Scene::Drawable _rotten, std::list<Scene::Drawable>* _drawables): FgObj(_drawable),
                                                                                                             rolled_over(_rotten), drawables(_drawables){};
//    void set_rotten_tree(Scene::Transform* _transform) : rotten(_transform) {};
    bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
    void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
};
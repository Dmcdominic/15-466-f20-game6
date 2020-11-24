#pragma once

#include "River.hpp"
#include "Scene.hpp"


struct Bridge : River {
    Scene::Drawable unactivated;
    Scene::Drawable bridge;
    Scene::Drawable *grass; 
    bool activated = false;

    // Methods
    Bridge(Scene::Drawable* _unactivated, Scene::Drawable _bridge, std::list<Scene::Drawable>* _drawables);
    bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
    void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
    void activate();
    void deactivate();
    void position_models();

};
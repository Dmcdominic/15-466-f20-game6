#pragma once

#include "River.hpp"
#include "Scene.hpp"


struct Bridge : River {
    Scene::Drawable unactivated;
    Scene::Drawable bridge;
    bool activated = false;

    // Methods
    Bridge(Scene::Drawable* _unactivated, Scene::Drawable _bridge, Scene::Drawable _toxic_water, std::list<Scene::Drawable>* _drawables):
    River( _unactivated, _toxic_water, _drawables), unactivated(*_unactivated), bridge(_bridge){};
    bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
    void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
    void activate();
    void deactivate();
};
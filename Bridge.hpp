#pragma once

#include "River.hpp"
#include "Scene.hpp"


struct Bridge : River {
    Scene::Drawable unactivated;
    Scene::Drawable unactivated_toxic;
    Scene::Drawable bridge;
    Scene::Drawable contaminated_bridge;
    bool activated = false;

    // Methods
    Bridge(Scene::Drawable* _unactivated, Scene::Drawable _unactivated_toxic,Scene::Drawable _water, Scene::Drawable _bridge, Scene::Drawable _toxic_bridge, std::list<Scene::Drawable>* _drawables):
    River(_unactivated, _unactivated_toxic, _water, _drawables), unactivated(*_unactivated), unactivated_toxic(_unactivated_toxic), bridge(_bridge), contaminated_bridge(_toxic_bridge){};
    bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
    void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
    void activate();
    void deactivate();
};
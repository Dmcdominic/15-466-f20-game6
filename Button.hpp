#pragma once

#include "Bridge.hpp"
#include "Scene.hpp"


struct Button : BgTile {
    Scene::Drawable unpressed = nullptr;
    Scene::Drawable pressed = nullptr;
    Scene::Drawable *grass = nullptr;
    Bridge* bridge;

  	Button(Scene *_scene, Bridge* _bridge); 
    void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
    void on_post_tick() override;
    void position_models() override;
};
#pragma once

#include "Bridge.hpp"
#include "Scene.hpp"


struct Button : BgTile {
    Scene::Drawable unpressed;
    Scene::Drawable pressed;
    Scene::Drawable grass;
    Bridge* bridge;

    Button(Scene::Drawable* _unpressed, Scene::Drawable _pressed, Scene::Drawable _grass, Bridge* _bridge):
    BgTile(_unpressed), unpressed(*_unpressed), pressed(_pressed), grass(_grass), bridge(_bridge){};
    void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
    void on_post_tick() override;
    void position_models();
};
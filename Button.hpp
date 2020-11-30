#pragma once

#include "Bridge.hpp"
#include "Scene.hpp"


struct Button : BgTile {
    // Fields
    glm::ivec2 bridge_pos = glm::ivec2();
    Scene::Drawable* pressed = nullptr;
    Scene::Drawable* grass = nullptr;

    private: Bridge* cached_bridge = nullptr;
    public:

    // Constructor
  	Button(Scene *_scene, glm::ivec2 _bridge_pos);

    // Methods
    void load_models(Scene* scene) override;
    Button* clone_lightweight(Cell* new_cell) override;

    void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
    void on_post_tick() override;

    Bridge *get_bridge(Grid* grid);

};
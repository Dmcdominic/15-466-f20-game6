#pragma once

#include "Grid.hpp"
#include "ModelLoader.hpp"
#include "Scene.hpp"


struct Turnstile : BgTile {
  // Constructors (inherited)
  //using BgTile::BgTile;
  Scene::Drawable *grass = nullptr;
  Turnstile(Scene *scene); 
  
  // Methods
  void on_post_tick() override;
  void position_models() override;
};
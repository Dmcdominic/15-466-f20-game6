#pragma once

#include "Grid.hpp"


struct Turnstile : BgTile {
  // Constructors (inherited)
  //using BgTile::BgTile;
  Scene::Drawable grass = nullptr;
  Turnstile(Scene::Drawable* _drawable, Scene::Drawable _grass):
  BgTile(_drawable), grass(_grass){};

  // Methods
  void on_post_tick() override;
  void position_models();
};
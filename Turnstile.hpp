#pragma once

#include "Grid.hpp"
#include "ModelLoader.hpp"
#include "Scene.hpp"


struct Turnstile : BgTile {
  // Constructors (inherited)
  using BgTile::BgTile;
  
  // Methods
  void load_models(Scene* scene) override;
  Turnstile* clone_lightweight(Cell* new_cell) override;

  void on_post_tick() override;
};
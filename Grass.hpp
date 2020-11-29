#pragma once

#include "Grid.hpp"
#include "ModelLoader.hpp"
#include "Scene.hpp"


struct Grass : BgTile {
  // Constructors (inherited)
  using BgTile::BgTile;

  // Methods
  void load_models(Scene* scene) override;
  Grass* clone_lightweight(Cell* new_cell) override;
};
#pragma once

#include "Grid.hpp"
#include "ModelLoader.hpp"
#include "Scene.hpp"


struct Mud : BgTile {
  // Constructors (inherited)
  using BgTile::BgTile;
  
  // Methods
  void load_models(Scene* scene) override;
  Mud* clone_lightweight(Cell* new_cell) override;

};
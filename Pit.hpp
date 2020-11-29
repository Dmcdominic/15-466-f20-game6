#pragma once

#include "Grid.hpp"
#include "ModelLoader.hpp"
#include "Scene.hpp"


struct Pit : BgTile {
  // Constructors (inherited)
  using BgTile::BgTile;

  // Methods
  void load_models(Scene* scene) override;
  Pit* clone_lightweight(Cell* new_cell) override;

  bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
};

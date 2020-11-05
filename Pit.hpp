#pragma once

#include "Grid.hpp"


struct Pit : BgTile {
  // Constructors (inherited)
  using BgTile::BgTile;

  // Methods
  bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
};

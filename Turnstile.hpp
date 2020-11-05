#pragma once

#include "Grid.hpp"


struct Turnstile : BgTile {
  // Constructors (inherited)
  using BgTile::BgTile;

  // Methods
  void on_post_tick() override;
};
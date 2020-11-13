#pragma once

#include "Grid.hpp"


struct Pit : BgTile {
  // Constructors (inherited)
  //using BgTile::BgTile;
  Scene::Drawable grass = nullptr;
  Pit(Scene::Drawable* _drawable, Scene::Drawable _grass):
  BgTile(_drawable), grass(_grass){};

  // Methods
  bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
  void position_models();
};

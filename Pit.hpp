#pragma once

#include "Grid.hpp"
#include "ModelLoader.hpp"
#include "Scene.hpp"


struct Pit : BgTile {
  // Constructors (inherited)
  //using BgTile::BgTile;
  Scene::Drawable *grass = nullptr;
  Pit(Scene *scene); 

  // Methods
  bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
  void position_models() override;
};

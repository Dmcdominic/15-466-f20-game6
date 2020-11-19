#pragma once

#include "Grid.hpp"
#include "Input.hpp"


// The tiles for paths between nodes for levels in the overworld
struct OverworldPath : BgTile {
  // Fields
  Scene::Drawable grass;

  // Constructors
  OverworldPath(Scene::Drawable* _drawable, Scene::Drawable _grass) : BgTile(_drawable), grass(_grass) {};

  // Methods
  void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
  void position_models();
};



// The node for each level in the overworld
struct OverworldNode : BgTile {
  // Fields
  Scene::Drawable grass;
  uint8_t level_index;

  // Constructors
  OverworldNode(Scene::Drawable* _drawable, Scene::Drawable _grass, uint8_t _level_index) : BgTile(_drawable), grass(_grass), level_index(_level_index) {};

  // Methods
  void position_models();
};

#pragma once

#include "Grid.hpp"
#include "Input.hpp"


// The tiles for paths between nodes for levels in the overworld
struct OverworldPath : BgTile {
  // Constructors (inherited)
  using BgTile::BgTile;

  // Methods
  void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
};



// The node for each level in the overworld
struct OverworldNode : BgTile {
  // Fields
  uint8_t level_index;

  // Constructors
  OverworldNode(Scene::Drawable* _drawable, uint8_t _level_index) : BgTile(_drawable), level_index(_level_index) {}
  OverworldNode(Scene::Drawable* _drawable, Cell* _cell, uint8_t _level_index) : BgTile(_drawable, _cell), level_index(_level_index) {}
};

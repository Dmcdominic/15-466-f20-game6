#pragma once

#include "Grid.hpp"
#include "Input.hpp"


// Either an OverworldPath or an OverworldNode
struct OverworldTile : BgTile {
  // Fields
  bool visited = false; // used for the level_index initialization search

  // Constructors (inherited)
  using BgTile::BgTile;

  // Methods
  virtual bool accessible() = 0;
};


// The tiles for paths between nodes for levels in the overworld
struct OverworldPath : OverworldTile {
  // Fields
  uint8_t max_adjacent_lvl = 0;
  bool faded = false;

  // Constructors (inherited)
  using OverworldTile::OverworldTile;

  // Methods
  void load_models(Scene* scene) override;
  OverworldPath* clone_lightweight(Cell* new_cell) override;

  void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
  void make_faded();
  bool accessible() override;
};



// The node for each level in the overworld
struct OverworldNode : OverworldTile {
  // Fields
  uint8_t level_index;

  // Constructors (inherited)
  using OverworldTile::OverworldTile;

  // Methods
  void load_models(Scene* scene) override;
  OverworldNode* clone_lightweight(Cell* new_cell) override;

  bool accessible() override;
};  

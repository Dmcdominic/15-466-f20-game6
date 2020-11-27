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
  Scene::Drawable *grass;

  uint8_t max_adjacent_lvl = 0;
  bool faded = false;

  // Constructors
  OverworldPath(Scene::Drawable* _drawable, Scene::Drawable *_grass) : OverworldTile(_drawable), grass(_grass) {};
  OverworldPath(Scene *scene); 

  // Methods
  void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
  void position_models();
  void make_faded();
  bool accessible() override;
  //uint8_t find_max_adj_lvl(Grid* grid);
};



// The node for each level in the overworld
struct OverworldNode : OverworldTile {
  // Fields
  Scene::Drawable *grass;
  uint8_t level_index;

  // Constructors
  OverworldNode(Scene::Drawable* _drawable, Scene::Drawable *_grass) : OverworldTile(_drawable), grass(_grass), level_index(0) {};
  OverworldNode(Scene *scene); 

  // Methods
  void position_models();
  bool accessible() override;
};  

#pragma once

#include "Grid.hpp"
#include "Input.hpp"


// Either an OverworldPath or an OverworldNode
struct OverworldTile : BgTile {
  // Fields
  bool visited = false; // used for the level_index initialization search
	Scene::Drawable* grass = nullptr;

  // Constructors (inherited)
  using BgTile::BgTile;
  // Methods
  void position_models() override;

  virtual bool accessible() = 0;
};


// The tiles for paths between nodes for levels in the overworld
struct OverworldPath : OverworldTile {
  // Fields
  uint8_t max_adjacent_lvl = 0;
  bool faded = false;
	bool left, right, upper, lower;

  // Constructors (inherited)
  using OverworldTile::OverworldTile;
	OverworldPath(Scene *scene, bool _left, bool _right, bool _upper, bool _lower) : left(_left), right(_right), upper(_upper), lower(_lower) {};


  // Methods
  //void load_models(Scene* scene) override;
  OverworldPath* clone_lightweight(Cell* new_cell) override;

  void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
  void load_models(Scene* scene) override;
  void make_faded();
  bool accessible() override;
};



// The node for each level in the overworld
struct OverworldNode : OverworldTile {
  // Fields
  uint8_t level_index;
  bool faded = false;
	Scene::Drawable* path = nullptr;

  // Constructors (inherited)
  using OverworldTile::OverworldTile;

  // Methods
  void load_models(Scene* scene) override;
  OverworldNode* clone_lightweight(Cell* new_cell) override;

  void position_models() override;
  void make_faded();
  bool accessible() override;
};  

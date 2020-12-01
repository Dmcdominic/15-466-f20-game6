#include "Overworld.hpp"
#include "Player.hpp"
#include "LitToxicColorTextureProgram.hpp"

#include "PlayMode.hpp"

#include <iostream>
// ----- OVERWORLD TILE -----
void OverworldTile::position_models() {
  //position the layered models
  CellItem::position_models();
  grass->transform->position = this->drawable->transform->position;
}


// ----- OVERWORLD PATH -----

// Load and shape to path direction
void OverworldPath::load_models(Scene* scene) {
  if (left && right && !upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("Path_Straight"));
    rotations = 1;
  }
  else if (!left && !right && upper && lower) {
    scene->drawables.push_back(model_loader->create_model("Path_Straight"));
  }
  else if (!left && right && !upper && lower) {
    scene->drawables.push_back(model_loader->create_model("Path_Bent"));
  }
  else if (!left && right && upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("Path_Bent"));
    rotations = 3;
  }
  else if (left && !right && upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("Path_Bent"));
    rotations = 2;
  }
  else if (left && !right && !upper && lower) {
    scene->drawables.push_back(model_loader->create_model("Path_Bent"));
    rotations = 1;
  }
  else if (!left && !right && upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("Path_End"));
  }
  else if (!left && right && !upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("Path_End"));
    rotations = 1;
  }
  else if (!left && !right && !upper && lower) {
    scene->drawables.push_back(model_loader->create_model("Path_End"));
    rotations = 2;
  }
  else if (left && !right && !upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("Path_End"));
    rotations = 3;
  }
  else {
    scene->drawables.push_back(model_loader->create_model("Path_None"));
  }
  this->drawable = &(scene->drawables.back());
  this->drawable->pipeline.set_uniforms = []() {
    glUniform1f(lit_toxic_color_texture_program->BROWN_AMT_float, 0.3f);
  };

  scene->drawables.push_back(model_loader->create_model("Grass"));
  this->grass = &scene->drawables.back();
}

// Create a copy with no drawables
OverworldPath* OverworldPath::clone_lightweight(Cell* new_cell) {
  OverworldPath* new_path = new OverworldPath(*this);
  new_path->cell = new_cell;
  new_path->drawable = nullptr;
  new_path->extra_drawables.clear();
  return new_path;
}


// When a player moves into a path tile, it should find the next path tile (or node) and send the player along.
void OverworldPath::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  BgTile::when_fg_obj_moved_into(objBeingMoved, displ);
  // If it's a player moving into this cell, tell it where to go next
  Player *player = dynamic_cast<Player*>(&objBeingMoved);
  if (player == nullptr) return;
  // Find the next OverworldTile
  glm::ivec2 prev_pos = cell->pos - displ;
  std::vector<glm::ivec2> displ_to_try = { glm::ivec2(1, 0), glm::ivec2(-1, 0), glm::ivec2(0, 1), glm::ivec2(0, -1) };
  for (auto displ_iter = displ_to_try.begin(); displ_iter != displ_to_try.end(); displ_iter++) {
    glm::ivec2 new_pos = cell->pos + *displ_iter;
    if (!current_grid->is_valid_pos(new_pos)) continue; // has to be a valid position
    if (new_pos == prev_pos) continue; // can't go backward
    OverworldTile* next_tile = dynamic_cast<OverworldTile*>(current_grid->cell_at(new_pos)->bgTile);
    if (next_tile == nullptr || !next_tile->accessible()) continue; // has to be an accessible OverworldTile
    player->next_forced_move = std::optional(*displ_iter);
    return;
  }
}


// Fade the path
void OverworldPath::make_faded() {
  faded = true;
  //Scene::Drawable path_faded = model_loader->create_model("Path_Faded");
  //path_faded.transform->position = this->drawable->transform->position;
  //delete this->drawable->transform;
  //*(this->drawable) = path_faded;
  this->drawable->pipeline.set_uniforms = []() {
    glUniform1f(lit_toxic_color_texture_program->BROWN_AMT_float, .0);
  };
}


// Returns true iff you should be able to path to this node.
bool OverworldPath::accessible() {
  return PlayMode::completed_level >= max_adjacent_lvl - 1;
}




// ----- OVERWORLD NODE -----

// Load any drawables
void OverworldNode::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model("Node"));
  this->drawable = &(scene->drawables.back());
  this->drawable->pipeline.set_uniforms = []() {
    glUniform1f(lit_toxic_color_texture_program->BROWN_AMT_float, 0.3f);
  };
  scene->drawables.push_back(model_loader->create_model("Grass"));
  this->grass = &scene->drawables.back();

}

void OverworldNode::position_models() {
  //position the layered models
  CellItem::position_models();
  grass->transform->position = this->drawable->transform->position;
}

// Fade the node
void OverworldNode::make_faded() {
  faded = true;
  // Scene::Drawable node_faded = model_loader->create_model("Node_Faded");
  // node_faded.transform->position = this->drawable->transform->position;
  // delete this->drawable->transform;
  // *(this->drawable) = node_faded;
  this->drawable->pipeline.set_uniforms = []() {
    glUniform1f(lit_toxic_color_texture_program->BROWN_AMT_float, .0);
  };
}

// Create a copy with no drawables
OverworldNode* OverworldNode::clone_lightweight(Cell* new_cell) {
  OverworldNode* new_node = new OverworldNode(*this);
  new_node->cell = new_cell;
  new_node->drawable = nullptr;
  new_node->extra_drawables.clear();
  return new_node;
}


// Returns true iff you should be able to path to this node.
bool OverworldNode::accessible() {
  return PlayMode::completed_level >= level_index - 1;
}

#include "Overworld.hpp"
#include "Player.hpp"

#include "PlayMode.hpp"

#include <iostream>


// ----- OVERWORLD PATH -----

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

OverworldPath::OverworldPath(Scene *scene) {
  scene->drawables.push_back(model_loader->create_model("Path")); 
  this->drawable = &(scene->drawables.back());
  scene->drawables.push_back(model_loader->create_model("Grass")); 
  this->grass = &(scene->drawables.back());
}

// Position the layered models
void OverworldPath::position_models() {
  if (grass->transform) {
    grass->transform->position = this->drawable->transform->position;
  }
}

// Fade the path
void OverworldPath::make_faded() {
  faded = true;
  Scene::Drawable path_faded = model_loader->create_model("Path_Faded");
  path_faded.transform->position = this->drawable->transform->position;
  delete this->drawable->transform;
  *(this->drawable) = path_faded;
}

// Returns true iff you should be able to path to this node.
bool OverworldPath::accessible() {
  return PlayMode::completed_level >= max_adjacent_lvl - 1;
}




// ----- OVERWORLD NODE -----
OverworldNode::OverworldNode(Scene *scene) {
  scene->drawables.push_back(model_loader->create_model("Node")); 
  this->drawable = &(scene->drawables.back());
  scene->drawables.push_back(model_loader->create_model("Grass")); 
  this->grass = &(scene->drawables.back());
}

// Position the layered models
void OverworldNode::position_models() {
  if (grass->transform) {
    grass->transform->position = this->drawable->transform->position;
  }
}

// Returns true iff you should be able to path to this node.
bool OverworldNode::accessible() {
  return PlayMode::completed_level >= level_index - 1;
}
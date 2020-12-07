#include "Player.hpp"
#include "Overworld.hpp"
#include "Ramp.hpp"

#include <iostream>


// Load any drawables
void Player::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model(get_model_name()));
  this->drawable = &(scene->drawables.back());

  scene->drawables.push_back(model_loader->create_model("Player_idle0"));
  this->idle0 = &(scene->drawables.back());
  this->extra_drawables.push_back(idle0);
  idle0->disabled = true;
}


// Create a copy with no drawables
Player* Player::clone_lightweight(Cell* new_cell) {
  Player* new_player = new Player(*this);
  new_player->cell = new_cell;
  new_player->drawable = nullptr;
  new_player->idle0 = nullptr;
  new_player->extra_drawables.clear();
  next_forced_move = std::nullopt;
  return new_player;
}


// For now, objects can't move into the player's cell
bool Player::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  return false;
}


// Controls the player's movement.
// If up/down/left/right is pressed, move in the grid.
// Also handles interaction logic (at least for overworld nodes).
// MUST CALL current_grid->pre_tick(); BEFORE UPDATING GRID BASED ON INPUT
bool Player::on_input(const Input& input, Output* output) {
  if (input.type == InputType::INTERACT) {
    OverworldNode *current_node = dynamic_cast<OverworldNode*>(cell->bgTile);
    if (current_node != nullptr) {
      current_grid->pre_tick();
      output->level_to_load = { current_node->level_index };
      output->last_node_pos = current_node->cell->pos;
      return true;
    }
    return false;
  }

  glm::ivec2 displ;
  int target_rotations = 0;
  switch (input.type) {
    case InputType::UP:
      displ = glm::ivec2( 0,  1);
      target_rotations = 0;
      break;
    case InputType::DOWN:
      displ = glm::ivec2( 0, -1);
      target_rotations = 2;
      break;
    case InputType::LEFT:
      displ = glm::ivec2(-1,  0);
      target_rotations = 3;
      break;
    case InputType::RIGHT:
      displ = glm::ivec2( 1,  0);
      target_rotations = 1;
      break;
    default:
      return false;
  }

  while (rotations % 4 != target_rotations) {
    rotate_90();
  }

  // Make sure we're looking at a valid position
  glm::ivec2 target_pos = cell->pos + displ;
  if (!current_grid->is_valid_pos(target_pos)) return false;

  // Check if we're moving into a ramp, and so should jump over 2 cells.
  Ramp *ramp = dynamic_cast<Ramp*>(current_grid->cell_at(target_pos)->fgObj);
  if (ramp != nullptr && ramp->should_slide_up(displ)) {
    displ *= 3;
    target_pos = cell->pos + displ;
    if (!current_grid->is_valid_pos(target_pos)) return false;
  }

  // Check if you're trying to move off of the Overworld path
  if (dynamic_cast<OverworldTile*>(cell->bgTile) != nullptr) {
    OverworldTile *targetTile = dynamic_cast<OverworldTile*>(current_grid->cell_at(target_pos)->bgTile);
    if (targetTile == nullptr || !targetTile->accessible()) return false;
  }

  // Actually try to move according to displ.
  // Based on FgObj::try_to_move_by()
  Cell* target_cell = current_grid->cell_at(target_pos);
  if (!target_cell->can_fg_obj_move_into(*this, displ)) return false;
  current_grid->pre_tick();
  target_cell->when_fg_obj_moved_into(*this, displ);
  if (current_grid->rolling) {
  	current_grid->to_be_moved.push_back(new RollItem(*this, target_cell));
  	return false;
  }
  if (target_cell->fgObj != nullptr) {
  	throw std::runtime_error("Trying to move player into a cell that seems to still have one");
  }
  target_cell->set_fg_obj(this);

  if (rand() < RAND_MAX / 3) {
    AudioManager::clips_to_play.push(AudioManager::AudioClip::FOOTSTEP);
  }
  else {
    AudioManager::clips_to_play.push(AudioManager::AudioClip::FOOTSTEP_SHORT);
  }

  return true;
}


// Animation
void Player::on_update() {
  // player idle animation
  idle_counter++;
  if (idle_counter > idle_num) {
    idle_counter = 0;
    if (player_state == 0) {
      player_state = 1;
      drawable->disabled = false;
      idle0->disabled = true;
    }
    else if (player_state == 1) {
      player_state = 0;
      drawable->disabled = true;
      idle0->disabled = false;
    }
  }
}

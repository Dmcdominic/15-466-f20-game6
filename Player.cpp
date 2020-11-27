#include "Player.hpp"
#include "Overworld.hpp"
#include "Ramp.hpp"

#include <iostream>

Player::Player(Scene *scene) : 
idle0(model_loader->create_model("Player")), 
idle1(model_loader->create_model("Player_idle0")) {
  scene->drawables.push_back(model_loader->create_model("Player")); 
  this->drawable = &(scene->drawables.back());
}

// For now, objects can't move into the player's cell
bool Player::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  return false;
}


// For now, does not apply
void Player::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
}


// Sky objects can move over the player
bool Player::can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ) {
  return true;
}


// Nothing happens
void Player::when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ) {
}


// Controls the player's movement.
// If up/down/left/right is pressed, move in the grid.
// Also handles interaction logic (at least for overworld nodes).
bool Player::on_input(const Input& input, Output* output) {
  if (input.type == InputType::INTERACT) {
    OverworldNode *current_node = dynamic_cast<OverworldNode*>(cell->bgTile);
    if (current_node != nullptr) {
      output->level_to_load = { current_node->level_index };
      return true;
    }
    return false;
  }

  glm::ivec2 displ;
  float roll;
  switch (input.type) {
    case InputType::UP:
      displ = glm::ivec2( 0,  1);
      roll = 0.0f;
      break;
    case InputType::DOWN:
      displ = glm::ivec2( 0, -1);
      roll = glm::pi<float>();
      break;
    case InputType::LEFT:
      displ = glm::ivec2(-1,  0);
      roll = glm::half_pi<float>();
      break;
    case InputType::RIGHT:
      displ = glm::ivec2( 1,  0);
      roll = glm::three_over_two_pi<float>();
      break;
    default:
      return false;
  }

  this->drawable->transform->rotation = glm::angleAxis(roll, glm::vec3(0.0f, 0.0f, 1.0f));

  // Make sure we're looking at a valid position
  glm::ivec2 target_pos = cell->pos + displ;
  if (!current_grid->is_valid_pos(target_pos)) return false;

  // Check if we're moving into a ramp, and so should jump over 2 cells.
  Ramp *ramp = dynamic_cast<Ramp*>(current_grid->cell_at(target_pos)->fgObj);
  if (ramp != nullptr && ramp->should_slide_up(displ)) {
    displ *= 3;
    glm::ivec2 target_pos = cell->pos + displ;
    if (!current_grid->is_valid_pos(target_pos)) return false;
  }

  // Check if you're trying to move off of the Overworld path
  if (dynamic_cast<OverworldTile*>(cell->bgTile) != nullptr) {
    OverworldTile *targetTile = dynamic_cast<OverworldTile*>(current_grid->cell_at(target_pos)->bgTile);
    if (targetTile == nullptr || !targetTile->accessible()) return false;
  }

  // Actually try to move according to displ
  if (try_to_move_by(displ)) {
    if (rand() > RAND_MAX / 2) {
      AudioManager::clips_to_play.push(AudioManager::AudioClip::FOOTSTEP);
    } else {
      AudioManager::clips_to_play.push(AudioManager::AudioClip::FOOTSTEP_SHORT);
    }
    return true;
  }
  return false;
}

#include "Barrel.hpp"
#include <iostream>


Scene::Drawable* Barrel::prefab;

// The barrel can only roll along its long axis
bool Barrel::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  // First check if displ is along the right axis for the barrel to roll
  if (( rollable_up_down && displ.x != 0) ||
      (!rollable_up_down && displ.y != 0)) {
    return false;
  }
  
  // Compute the target position and check if it will end up outside the grid
  glm::ivec2 target_pos = this->cell->pos + displ;
  if (!current_grid->is_valid_pos(target_pos)) {
    return false;
  }
  // Otherwise, check if this can be pushed according to displ.
  return current_grid->cells.at(target_pos.x).at(target_pos.y).can_fg_obj_move_into(*this, displ);
}


// TODO - this might change its rotation? Or should the hill change its rotation?
void Barrel::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  if (!try_to_move_by(displ)) {
    throw std::runtime_error("when_fg_obj_moved_into() somehow called for an object position & displacement that COULDN'T move.");
  }
}


// Called right after on_input for each cell
//void Barrel::on_post_tick(){
//  // testing - rotates
//  rotate_90();
//}


// Rotates the barrel 90 degrees, so its rollable-axis is switched now
void Barrel::rotate_90() {
  rollable_up_down = !rollable_up_down;
  float angle = rollable_up_down ? 0 : glm::half_pi<float>();
  this->drawable->transform->rotation = glm::angleAxis(angle, glm::vec3(0, 0, 1));
  // TODO
}


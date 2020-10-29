#include "Player.hpp"


bool Player::can_fg_obj_move_into(const FgObj& objBeingMoved, const glm::ivec2& displ) {
  return false;
}

void Player::when_fg_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  // Does not apply
}

bool Player::on_input() {
  // TODO - if up/down/left/right (WASD) is pressed, move in the grid
  return false;
}

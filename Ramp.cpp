#include "Ramp.hpp"
#include <iostream>


// If the ramp gets rotated, we need to rotate our up direction too.
void Ramp::rotate_90() {
  up_dir = glm::ivec2(up_dir.y, -up_dir.x);
  CellItem::rotate_90();
}


// Returns true iff a player moving into the ramp according to displ should go UP the ramp.
bool Ramp::should_slide_up(const glm::ivec2& displ) {
  return displ == up_dir;
}

#include "FixedRock.hpp"
#include <iostream>


Scene::Drawable* FixedRock::prefab;

// Fixed rock can't be displaced.
bool FixedRock::can_fg_obj_move_into(const FgObj& objBeingMoved, const glm::ivec2& displ) {
  return false;
}

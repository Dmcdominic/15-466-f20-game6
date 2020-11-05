#include "Tree.hpp"
#include <iostream>


//Scene::Drawable* FixedRock::prefab;

// Fixed rock can't be displaced.
bool Tree::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  return false;
}

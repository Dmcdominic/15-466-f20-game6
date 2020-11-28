#include "Barrel.hpp"
#include <iostream>

Barrel::Barrel(Scene *scene) {
  scene->drawables.push_back(model_loader->create_model("Barrel")); 
  this->drawable = &(scene->drawables.back());
}

// The barrel can only roll along its long axis.
bool Barrel::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  // First check if displ is along the right axis for the barrel to roll.
  if (( rollable_up_down && displ.x != 0) ||
      (!rollable_up_down && displ.y != 0)) {
    return false;
  }
  // If that's all good, then use default pushable behavior.
  return FgObj::can_fg_obj_move_into(objBeingMoved, displ);
}


// If the barrel gets rotated, we need to flip our rollable axis bool too.
void Barrel::rotate_90() {
  rollable_up_down = !rollable_up_down;
  CellItem::rotate_90();
}


// Returns the audio clip that should be played when this object is moved
std::optional<AudioManager::AudioClip> Barrel::get_move_clip() {
  return std::optional(AudioManager::AudioClip::ROLL_BARREL);
}

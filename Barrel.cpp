#include "Barrel.hpp"
#include <iostream>

Barrel::Barrel(Scene *scene, int _rotations) : FgObj(scene, _rotations) {}


// Load all drawables
void Barrel::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model(get_model_name()));
  this->drawable = &(scene->drawables.back());
};


// Create a copy with no drawables
Barrel* Barrel::clone_lightweight(Cell* new_cell) {
  Barrel* new_barrel = new Barrel(*this);
  new_barrel->cell = new_cell;
  new_barrel->drawable = nullptr;
  new_barrel->extra_drawables.clear();
  return new_barrel;
}


// The barrel can only roll along its long axis.
bool Barrel::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  // First check if displ is along the right axis for the barrel to roll.
  if (( rollable_up_down() && displ.x != 0) ||
      (!rollable_up_down() && displ.y != 0)) {
    return false;
  }
  // If that's all good, then use default pushable behavior.
  return FgObj::can_fg_obj_move_into(objBeingMoved, displ);
}


// Returns the audio clip that should be played when this object is moved
std::optional<AudioManager::AudioClip> Barrel::get_move_clip() {
  return std::optional(AudioManager::AudioClip::ROLL_BARREL);
}


// True = rollable_up_down, False = rollable left/right
bool Barrel::rollable_up_down() {
  return rotations % 2 == 0;
}

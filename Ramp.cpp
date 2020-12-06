#include "Ramp.hpp"
#include <iostream>


// Load all drawables
void Ramp::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model(get_model_name()));
  this->drawable = &(scene->drawables.back());
};


// Create a copy with no drawables
Ramp* Ramp::clone_lightweight(Cell* new_cell) {
  Ramp* new_ramp = new Ramp(*this);
  new_ramp->cell = new_cell;
  new_ramp->drawable = nullptr;
  new_ramp->extra_drawables.clear();
  return new_ramp;
}


// Returns true iff a player moving into the ramp according to displ should go UP the ramp.
bool Ramp::should_slide_up(const glm::ivec2& displ) {
  glm::ivec2 up_dir = glm::ivec2(0, 1);
  for (int i = 0; i < rotations; i++) {
    up_dir = glm::ivec2(up_dir.y, -up_dir.x);
  }
  return displ == up_dir;
}


// Returns the audio clip that should be played when this object is moved
std::optional<AudioManager::AudioClip> Ramp::get_move_clip() {
  return std::optional(AudioManager::AudioClip::DRAG_STONE);
}

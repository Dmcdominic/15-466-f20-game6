#include "Acorn.hpp"
#include <iostream>


// Load any drawables
void Acorn::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model(get_model_name()));
  this->drawable = &(scene->drawables.back());
}


// Create a copy with no drawables
Acorn* Acorn::clone_lightweight(Cell* new_cell) {
  Acorn* new_acorn = new Acorn(*this);
  new_acorn->cell = new_cell;
  new_acorn->drawable = nullptr;
  new_acorn->extra_drawables.clear();
  return new_acorn;
}


// Returns the audio clip that should be played when this object is moved
std::optional<AudioManager::AudioClip> Acorn::get_move_clip() {
  return std::optional(AudioManager::AudioClip::DRAG_STONE);
}

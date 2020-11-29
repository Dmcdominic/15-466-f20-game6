#include "Rock.hpp"
#include <iostream>


// Load any drawables
void Rock::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model("Rock"));
  this->drawable = &(scene->drawables.back());
}


// Create a copy with no drawables
Rock* Rock::clone_lightweight(Cell* new_cell) {
  Rock* new_rock = new Rock(*this);
  new_rock->cell = new_cell;
  new_rock->drawable = nullptr;
  new_rock->extra_drawables.clear();
  return new_rock;
}


// Returns the audio clip that should be played when this object is moved
std::optional<AudioManager::AudioClip> Rock::get_move_clip() {
  return std::optional(AudioManager::AudioClip::DRAG_STONE);
}

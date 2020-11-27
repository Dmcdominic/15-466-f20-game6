#include "Rock.hpp"
#include <iostream>


// No need to override anything. Pushable is default FgObj behavior.

Rock::Rock(Scene *scene) {
  scene->drawables.push_back(model_loader->create_model("Rock")); 
  this->drawable = &(scene->drawables.back());
}

// Returns the audio clip that should be played when this object is moved
std::optional<AudioManager::AudioClip> Rock::get_move_clip() {
  return std::optional(AudioManager::AudioClip::DRAG_STONE);
}

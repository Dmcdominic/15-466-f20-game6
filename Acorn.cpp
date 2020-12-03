#include "Acorn.hpp"
#include <iostream>


// Load any drawables
void Acorn::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model(get_model_name()));
  this->drawable = &(scene->drawables.back());
  
	scene->drawables.push_back(model_loader->create_model("Sapling"));
	sapling = &(scene->drawables.back());
  sapling->disabled = true; 
	this->extra_drawables.push_back(sapling);
}


// Create a copy with no drawables
Acorn* Acorn::clone_lightweight(Cell* new_cell) {
  Acorn* new_acorn = new Acorn(*this);
  new_acorn->cell = new_cell;
  new_acorn->drawable = nullptr;
  new_acorn->extra_drawables.clear();
  return new_acorn;
}

void Acorn::sprout() {
  sprouted = true; 
  sapling->disabled = false; 
  this->drawable->disabled = true; 

  //TODO: balance this? 
  current_grid->grid_environment_score += 5;

}

bool Acorn::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ){
  if(sprouted) {
    return false; 
  }
  else return FgObj::can_fg_obj_move_into(objBeingMoved, displ); 
}

// Returns the audio clip that should be played when this object is moved
std::optional<AudioManager::AudioClip> Acorn::get_move_clip() {
  return std::optional(AudioManager::AudioClip::DRAG_STONE);
}

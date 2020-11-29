#include "Grass.hpp"

#include <iostream>


// Load any drawables
void Grass::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model("Grass"));
  this->drawable = &(scene->drawables.back());
}


// Create a copy with no drawables
Grass* Grass::clone_lightweight(Cell* new_cell) {
  Grass* new_grass = new Grass(*this);
  new_grass->cell = new_cell;
  new_grass->drawable = nullptr;
  new_grass->extra_drawables.clear();
  return new_grass;
}
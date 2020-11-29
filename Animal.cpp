#include "Animal.hpp"
#include <iostream>


// Load any drawables
void Animal::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model("Animal"));
  this->drawable = &(scene->drawables.back());
}


// Create a copy with no drawables
Animal* Animal::clone_lightweight(Cell* new_cell) {
  Animal* new_animal = new Animal(*this);
  new_animal->cell = new_cell;
  new_animal->drawable = nullptr;
  new_animal->extra_drawables.clear();
  return new_animal;
}

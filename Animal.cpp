#include "Animal.hpp"
#include <iostream>


// No need to override anything. Pushable is default FgObj behavior.

Animal::Animal(Scene *scene) {
  scene->drawables.push_back(model_loader->create_model("Animal")); 
  this->drawable = &(scene->drawables.back());
}

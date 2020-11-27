#include "Grass.hpp"

Grass::Grass(Scene *scene) {
  scene->drawables.push_back(model_loader->create_model("Grass")); 
  this->drawable = &(scene->drawables.back());
}

#include "Cloud.hpp"
#include <iostream>


// Constructor
Cloud::Cloud(Scene *scene) {}


// Load any drawables
void Cloud::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model("Cloud"));
  this->drawable = &(scene->drawables.back());
}


// Create a copy with no drawables
Cloud* Cloud::clone_lightweight(Cell* new_cell) {
  Cloud* new_cloud = new Cloud(*this);
  new_cloud->cell = new_cell;
  new_cloud->drawable = nullptr;
  new_cloud->extra_drawables.clear();
  return new_cloud;
}

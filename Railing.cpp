#include "Railing.hpp"


// Load any drawables
void Railing::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model(get_model_name()));
  this->drawable = &(scene->drawables.back());
}


// Create a copy with no drawables
Railing* Railing::clone_lightweight(Cell* new_cell) {
  Railing* new_railing = new Railing(*this);
  new_railing->cell = new_cell;
  new_railing->drawable = nullptr;
  new_railing->extra_drawables.clear();
  return new_railing;
}


bool Railing::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
    return false;
}
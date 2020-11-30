#include "Railing.hpp"

// Constructor
Railing::Railing(Scene * scene, int _rotations, bool _left, bool _right, bool _upper, bool _lower) : FgObj(scene, _rotations), left(_left), right(_right), upper(_upper), lower(_lower) {}


// Load any drawables
void Railing::load_models(Scene* scene) {
  if (!left && right && !upper && lower) {
    scene->drawables.push_back(model_loader->create_model("Railing_Bent"));
  }
  else if (!left && right && upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("Railing_Bent"));
  }
  else if (left && !right && upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("Railing_Bent"));
  }
  else if (left && !right && !upper && lower) {
    scene->drawables.push_back(model_loader->create_model("Railing_Bent"));
  }
  else {
    scene->drawables.push_back(model_loader->create_model("Railing_Straight"));
  }
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
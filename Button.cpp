#include "Button.hpp"
#include <iostream>


// Constructor
Button::Button(Scene* scene, glm::ivec2 _bridge_pos) : BgTile(scene), bridge_pos(_bridge_pos) {}


// Load any drawables
void Button::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model("Button"));
  this->drawable = &(scene->drawables.back());

  scene->drawables.push_back(model_loader->create_model("Button_Pushed"));
  this->pressed = &(scene->drawables.back());
  this->extra_drawables.push_back(this->pressed);
  this->pressed->disabled = true;

  scene->drawables.push_back(model_loader->create_model("Grass"));
  this->grass = &(scene->drawables.back());
  this->extra_drawables.push_back(this->grass);
}


// Create a copy with no drawables
Button* Button::clone_lightweight(Cell* new_cell) {
  Button* new_button = new Button(*this);
  new_button->cell = new_cell;
  new_button->drawable = nullptr;
  new_button->pressed = nullptr;
  new_button->grass = nullptr;
  new_button->extra_drawables.clear();
  return new_button;
}


void Button::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
    this->drawable->disabled = true;
    this->pressed->disabled = false;
    get_bridge()->activate();
}


void Button::on_post_tick(){
  if (this->cell->fgObj==nullptr && this->get_bridge()->activated) {
      this->drawable->disabled = false;
      this->pressed->disabled = true;
      get_bridge()->deactivate();
  }
}


// Returns the bridge corresponding to this button.
// Throws a runtime error if it doesn't exist.
Bridge* Button::get_bridge() {
  if (cached_bridge == nullptr) {
    BgTile* bgTile = current_grid->cell_at(bridge_pos)->bgTile;
    cached_bridge = dynamic_cast<Bridge*>(bgTile);
    if (cached_bridge == nullptr) throw std::runtime_error("Button couldn't find a bridge at the given bridge_pos");
  }
  return cached_bridge;
}

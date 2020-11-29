#include "Turnstile.hpp"


// Load any drawables
void Turnstile::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model("Turnstile"));
  this->drawable = &(scene->drawables.back());

  scene->drawables.push_back(model_loader->create_model("Grass"));
  this->extra_drawables.push_back(&scene->drawables.back());
}


// Create a copy with no drawables
Turnstile* Turnstile::clone_lightweight(Cell* new_cell) {
  Turnstile* new_turnstile = new Turnstile(*this);
  new_turnstile->cell = new_cell;
  new_turnstile->drawable = nullptr;
  new_turnstile->extra_drawables.clear();
  return new_turnstile;
}


// Called right after on_input for each cell
void Turnstile::on_post_tick(){
  rotate_90();
  if (cell->fgObj != nullptr) {
    cell->fgObj->rotate_90();
  }
}

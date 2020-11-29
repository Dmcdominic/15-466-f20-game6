#include "Pit.hpp"
#include "Player.hpp"



// Load any drawables
void Pit::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model("Pit"));
  this->drawable = &(scene->drawables.back());
  scene->drawables.push_back(model_loader->create_model("Grass"));
  this->extra_drawables.push_back(&scene->drawables.back());
}


// Create a copy with no drawables
Pit* Pit::clone_lightweight(Cell* new_cell) {
  Pit* new_pit = new Pit(*this);
  new_pit->cell = new_cell;
  new_pit->drawable = nullptr;
  new_pit->extra_drawables.clear();
  return new_pit;
}


// Players can't move into pits. All other FgObjs can.
bool Pit::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  if (dynamic_cast<Player*>(&objBeingMoved) != nullptr) return false;
  return BgTile::can_fg_obj_move_into(objBeingMoved, displ);
}

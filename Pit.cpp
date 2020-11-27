#include "Pit.hpp"
#include "Player.hpp"

Pit::Pit(Scene *scene) {
  scene->drawables.push_back(model_loader->create_model("Pit")); 
  this->drawable = &(scene->drawables.back());
  scene->drawables.push_back(model_loader->create_model("Grass")); 
  this->grass = &(scene->drawables.back());
}

bool Pit::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  if (dynamic_cast<Player*>(&objBeingMoved) != nullptr) return false;
  return BgTile::can_fg_obj_move_into(objBeingMoved, displ);
}
void Pit::position_models() {
    //position the layered models
	if(grass)
    	grass->transform->position = this->drawable->transform->position;
}
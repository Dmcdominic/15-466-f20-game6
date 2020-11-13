#include "Turnstile.hpp"


// Called right after on_input for each cell
void Turnstile::on_post_tick(){
  rotate_90();
  if (cell->fgObj != nullptr) {
    cell->fgObj->rotate_90();
  }
}
void Turnstile::position_models() {
    //position the layered models
	if(&grass)
    	grass.transform->position = this->drawable->transform->position;
}
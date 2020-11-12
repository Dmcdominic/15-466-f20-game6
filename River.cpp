#include "River.hpp"
#include "Barrel.hpp"
#include "Player.hpp"


bool River::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ){
  if (dynamic_cast<Player*>(&objBeingMoved) != nullptr) return false;
  return BgTile::can_fg_obj_move_into(objBeingMoved, displ);
}


void River::contaminated() {
	this->rotten.transform->position = this->drawable->transform->position;
    delete this->cell->bgTile->drawable->transform;
	*(this->cell->bgTile->drawable) = this->rotten;
}


void River::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
	if(dynamic_cast<Barrel*>(&objBeingMoved) != nullptr) {
		this->rotten.transform->position = this->drawable->transform->position;
        delete this->cell->bgTile->drawable->transform;
		*(this->cell->bgTile->drawable) = this->rotten;
		for(size_t i = 0; i < tiles->size(); i++) {
			(*tiles)[i]->contaminated();
		}
	}
}
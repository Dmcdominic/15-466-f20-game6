#include "River.hpp"
#include "Barrel.hpp"

Scene::Drawable* River::prefab;

bool River::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ){
	return (dynamic_cast<Barrel*>(&objBeingMoved) != nullptr);
}

void River::contaminated() {
	this->rotten.transform->position = this->drawable->transform->position;
	*(this->cell->bgTile->drawable) = this->rotten;
}

void River::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
	if(dynamic_cast<Barrel*>(&objBeingMoved) != nullptr) {
		this->rotten.transform->position = this->drawable->transform->position;
		*(this->cell->bgTile->drawable) = this->rotten;
		for(size_t i = 0; i < tiles->size(); i++) {
			(*tiles)[i]->contaminated();
		}
	}
}
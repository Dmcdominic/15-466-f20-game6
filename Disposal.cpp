#include "Disposal.hpp"
#include "Barrel.hpp"

Scene::Drawable* Disposal::prefab;

bool Disposal::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ){
	return (dynamic_cast<Barrel*>(&objBeingMoved) != nullptr);
}


void Disposal::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
	if(dynamic_cast<Barrel*>(&objBeingMoved) != nullptr) {
        //add points
        current_grid->num_disposed ++; 
        //delete the barrel
        this->drawables->remove(*(objBeingMoved.drawable));
        this->cell->set_fg_obj(nullptr);

	}
}
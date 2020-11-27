#include "Disposal.hpp"
#include "Barrel.hpp"


Disposal::Disposal(Scene *_scene) : scene(_scene) {
    scene->drawables.push_back(model_loader->create_model("Disposal")); 
    this->drawable = &(scene->drawables.back());
    scene->drawables.push_back(model_loader->create_model("Grass")); 
    this->grass = &(scene->drawables.back());
}

bool Disposal::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ){
	return (dynamic_cast<Barrel*>(&objBeingMoved) != nullptr);
}


void Disposal::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
	if(dynamic_cast<Barrel*>(&objBeingMoved) != nullptr) {
        //add points
        current_grid->num_disposed ++; 
        //delete the barrel
        scene->drawables.remove(*(objBeingMoved.drawable));
        this->cell->set_fg_obj(nullptr);
        //TODO: delete fg object (memory leak?)
	}
}

void Disposal::position_models() {
    //position the layered models
	if(grass)
    	grass->transform->position = this->drawable->transform->position;
}
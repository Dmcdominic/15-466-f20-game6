#include "Bridge.hpp"
#include "Barrel.hpp"
#include <iostream>


Bridge::Bridge(Scene *scene) : River(scene, true, true, false, false),
unactivated(model_loader->create_model("Bridge_Unactivated")),
bridge(model_loader->create_model("Bridge")){
    scene->drawables.push_back(model_loader->create_model("River_Straight")); 
    grass = &(scene->drawables.back());
    scene->drawables.push_back(unactivated); 
    this->drawable = &(scene->drawables.back());

}


void Bridge::position_models(){
    if(water->transform)
		water->transform->position = this->drawable->transform->position;
    if(grass->transform){
		grass->transform->position = this->drawable->transform->position;
		grass->transform->rotation = this->drawable->transform->rotation;
    }
}

bool Bridge::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ){
    if (activated) return true;
    return River::can_fg_obj_move_into(objBeingMoved, displ);
}

void Bridge::activate() {
    activated = true;
    this->bridge.transform->position = this->drawable->transform->position;
    this->bridge.transform->rotation = this->drawable->transform->rotation;
//    delete this->drawable->transform;
    *(this->drawable) = this->bridge;
}

void Bridge::deactivate() {
    activated = false;
    this->bridge.transform->position = this->drawable->transform->position;
    this->bridge.transform->rotation = this->drawable->transform->rotation;
//    delete this->drawable->transform;
    *(this->drawable) = this->unactivated;
    
    //sink object if it is on top
    if(dynamic_cast<Barrel*>(this->cell->fgObj) != nullptr) {
        iscontaminated = true;
        current_grid->grid_environment_score -= 5;

        // delete this->water->transform;
        // *water = (model_loader->create_model("Water_Toxic"));
        // water->transform->position = this->drawable->transform->position;
        set_purple_amt(0.5); 

    }

    if(this->cell->fgObj) just_sunk = true; 
}

void Bridge::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
    if (!activated){
        River::when_fg_obj_moved_into(objBeingMoved, displ);
    }
}
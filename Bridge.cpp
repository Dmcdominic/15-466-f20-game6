#include "Bridge.hpp"
#include "Barrel.hpp"
#include <iostream>


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
        current_grid->environment_score -= 5;
        this->rotten.transform->position = this->drawable->transform->position;
        delete this->water->transform;
        *water = this->rotten;
    }

    if(this->cell->fgObj) just_sunk = true; 
}

void Bridge::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
    if (!activated){
        River::when_fg_obj_moved_into(objBeingMoved, displ);
    }
}
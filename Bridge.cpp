#include "Bridge.hpp"
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
}

void Bridge::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
    if (!activated){
        River::when_fg_obj_moved_into(objBeingMoved, displ);
    }
}
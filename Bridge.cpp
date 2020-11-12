#include "Bridge.hpp"
#include <iostream>


bool Bridge::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ){
    return activated;
}

void Bridge::activate() {
    activated = true;
    this->bridge.transform->position = this->drawable->transform->position;
    this->contaminated_bridge.transform->position = this->rotten.transform->position;
//    delete this->drawable->transform;
    *(this->drawable) = this->bridge;
    this->rotten = this->contaminated_bridge;
}

void Bridge::deactivate() {
    activated = false;
    this->bridge.transform->position = this->drawable->transform->position;
    this->contaminated_bridge.transform->position = this->rotten.transform->position;
//    delete this->drawable->transform;
    *(this->drawable) = this->unactivated;
    this->rotten = this->unactivated_toxic;
}

void Bridge::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
    if (!activated){
        River::when_fg_obj_moved_into(objBeingMoved, displ);
    }
}
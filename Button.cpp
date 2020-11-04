#include "Button.hpp"
#include <iostream>


void Button::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
    this->pressed.transform->position = this->drawable->transform->position;
    *(this->drawable) = this->pressed;
    bridge->activate();
}
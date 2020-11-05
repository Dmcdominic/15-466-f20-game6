#include "Button.hpp"
#include <iostream>


void Button::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
    this->pressed.transform->position = this->drawable->transform->position;
//    delete this->drawable->transform;
    *(this->drawable) = this->pressed;
    bridge->activate();
}

void Button::on_post_tick(){
  if (this->cell->fgObj==nullptr&&this->bridge->activated) {
      this->unpressed.transform->position = this->drawable->transform->position;
      *(this->drawable) = this->unpressed;
      bridge->deactivate();
  }
}
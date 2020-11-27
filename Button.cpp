#include "Button.hpp"
#include <iostream>

Button::Button(Scene *scene, Bridge* _bridge) : bridge(_bridge),
pressed(model_loader->create_model("Button")),
unpressed(model_loader->create_model("Button_Pushed")) {
    scene->drawables.push_back(this->pressed); 
    this->drawable = &(scene->drawables.back());
    scene->drawables.push_back(model_loader->create_model("Grass")); 
    this->grass = &(scene->drawables.back());
} 

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

void Button::position_models() {
    //position the layered models
	if(grass->transform)
        grass->transform->position = this->drawable->transform->position;
}
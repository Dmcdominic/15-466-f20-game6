#include "Bridge.hpp"
#include "Barrel.hpp"
#include <iostream>


// Constructor
Bridge::Bridge(Scene* scene, bool vertical) : River(scene, vertical, vertical, !vertical, !vertical) {}


// Load any drawables
void Bridge::load_models(Scene* scene) {
  River::load_models(scene);

  scene->drawables.push_back(model_loader->create_model("Bridge_Unactivated"));
  unactivated = &(scene->drawables.back());
  extra_drawables.push_back(unactivated);

  scene->drawables.push_back(model_loader->create_model("Bridge"));
  bridge = &(scene->drawables.back());
  extra_drawables.push_back(bridge);

  set_bridge_drawables();
}


void Bridge::position_models(){
    River::position_models();
}


// Create a copy with no drawables
Bridge* Bridge::clone_lightweight(Cell* new_cell) {
  Bridge* new_bridge = new Bridge(*this);
  new_bridge->cell = new_cell;
  // TODO - maybe call some River helper function cuz it's doing similar stuff?
  new_bridge->drawable = nullptr;
  new_bridge->water = nullptr;
  new_bridge->sunk_object = nullptr;
  new_bridge->unactivated = nullptr;
  new_bridge->bridge = nullptr;
  new_bridge->extra_drawables.clear();
  return new_bridge;
}


bool Bridge::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ){
    if (activated) return true;
    return River::can_fg_obj_move_into(objBeingMoved, displ);
}


void Bridge::activate() {
    activated = true;
    set_bridge_drawables();
}


void Bridge::deactivate() {
    activated = false;
    set_bridge_drawables();
    
    //sink object if it is on top
    if (this->cell->fgObj != nullptr) {
      try_to_sink(*this->cell->fgObj);
    }
}


// Sets the unactivated and activated bridge drawables disabled according to current activated state.
void Bridge::set_bridge_drawables() {
  if (unactivated != nullptr) unactivated->disabled = activated;
  //if (unactivated != nullptr) unactivated->transform->position = glm::vec3(0.0f, 0.0f, 3.0f);
  if (bridge != nullptr) bridge->disabled = !activated;
  //if (bridge != nullptr) bridge->transform->position = glm::vec3(0.0f, 0.0f, 3.0f);
}


void Bridge::rotate_90(bool skip_incr) {
  CellItem::rotate_90(skip_incr);
}


void Bridge::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
    if (!activated){
        River::when_fg_obj_moved_into(objBeingMoved, displ);
    }
}
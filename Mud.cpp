#include "Mud.hpp"
#include "Acorn.hpp"


// Load any drawables
void Mud::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model("Mud"));
  this->drawable = &(scene->drawables.back());

  scene->drawables.push_back(model_loader->create_model("Grass"));
  this->extra_drawables.push_back(&scene->drawables.back());

}


// Create a copy with no drawables
Mud* Mud::clone_lightweight(Cell* new_cell) {
  Mud* new_mud = new Mud(*this);
  new_mud->cell = new_cell;
  new_mud->drawable = nullptr;
  new_mud->extra_drawables.clear();
  return new_mud;
}



void Mud::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
	if(dynamic_cast<Acorn*>(&objBeingMoved) != nullptr) {
        dynamic_cast<Acorn*>(&objBeingMoved)->sprout(); 
	}
}
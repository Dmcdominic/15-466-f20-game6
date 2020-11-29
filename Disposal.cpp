#include "Disposal.hpp"
#include "Barrel.hpp"


// Constructor
Disposal::Disposal(Scene *scene) : BgTile(scene) {}


// Load any drawables
void Disposal::load_models(Scene* scene) {
  scene->drawables.push_back(model_loader->create_model("Disposal"));
  this->drawable = &(scene->drawables.back());

  scene->drawables.push_back(model_loader->create_model("Grass"));
  this->grass = &(scene->drawables.back());
  this->extra_drawables.push_back(grass);
}


// Create a copy with no drawables
Disposal* Disposal::clone_lightweight(Cell* new_cell) {
  Disposal* new_disposal = new Disposal(*this);
  new_disposal->cell = new_cell;
  new_disposal->drawable = nullptr;
  new_disposal->grass = nullptr;
  new_disposal->extra_drawables.clear();
  return new_disposal;
}


bool Disposal::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ){
	return (dynamic_cast<Barrel*>(&objBeingMoved) != nullptr);
}


void Disposal::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
	if(dynamic_cast<Barrel*>(&objBeingMoved) != nullptr) {
        //add points
        current_grid->num_disposed++; 
        //delete the barrel
        //scene->drawables.remove(*(objBeingMoved.drawable));
        //this->cell->set_fg_obj(nullptr);
        //TODO: delete fg object (memory leak?)
	}
}
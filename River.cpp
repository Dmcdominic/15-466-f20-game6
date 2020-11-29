#include "River.hpp"
#include "Barrel.hpp"
#include "Player.hpp"
#include "LitPurpleColorTextureProgram.hpp"

#include <iostream>


// Constructor
River::River(Scene *scene, bool _left, bool _right, bool _upper, bool _lower) : left(_left), right(_right), upper(_upper), lower(_lower) {}


// Load all drawables
void River::load_models(Scene* scene) {
  if (!left && !right && !upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("River_Single"));
  }
  else if (left && right && !upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("River_Straight"));
    rotations = 1;
  }
  else if (!left && !right && upper && lower) {
    scene->drawables.push_back(model_loader->create_model("River_Straight"));
  }
  else if (!left && right && !upper && lower) {
    scene->drawables.push_back(model_loader->create_model("River_Bent"));
  }
  else if (!left && right && upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("River_Bent"));
    rotations = 3;
  }
  else if (left && !right && upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("River_Bent"));
    rotations = 2;
  }
  else if (left && !right && !upper && lower) {
    scene->drawables.push_back(model_loader->create_model("River_Bent"));
    rotations = 1;
  }
  else if (!left && !right && upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("River_End"));
  }
  else if (!left && right && !upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("River_End"));
    rotations = 1;
  }
  else if (!left && !right && !upper && lower) {
    scene->drawables.push_back(model_loader->create_model("River_End"));
    rotations = 2;
  }
  else if (left && !right && !upper && !lower) {
    scene->drawables.push_back(model_loader->create_model("River_End"));
    rotations = 3;
  }
  else {
    scene->drawables.push_back(model_loader->create_model("River_None"));
  }
  this->drawable = &(scene->drawables.back());

  scene->drawables.push_back(model_loader->create_model("Water"));
  water = &(scene->drawables.back());
  extra_drawables.push_back(water);
}


// Set the transforms of all drawables appropriately
void River::position_models() {
  //position the layered models
  CellItem::position_models();
}


// Create a copy with no drawables
River* River::clone_lightweight(Cell* new_cell) {
  River* new_river = new River(*this);
  new_river->cell = new_cell;
  new_river->drawable = nullptr;
  new_river->water = nullptr;
  new_river->sunk_object = nullptr; // TODO
  new_river->tiles->clear(); // TODO
  new_river->extra_drawables.clear();
  return new_river;
}



void River::set_purple_amt(float amt) {
    this->water->pipeline.set_uniforms = [amt](){
        glUniform1f(lit_purple_color_texture_program->PURPLE_AMT_float, amt);
    };
}


void River::contaminated() {
    current_grid->grid_environment_score -= 5;
    iscontaminated += 1;
    willbecontaminated = false;
    // delete this->water->transform;
    // *water = (model_loader->create_model("Water_Toxic"));
    // water->transform->position = this->drawable->transform->position;
    if (iscontaminated==1) set_purple_amt(0.5);
    if (iscontaminated==2) set_purple_amt(1.0);
}


bool River::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  if (!sunk_object && dynamic_cast<Player*>(&objBeingMoved) != nullptr) return false;
  return BgTile::can_fg_obj_move_into(objBeingMoved, displ);
}


void River::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
  try_to_sink(objBeingMoved);
}


// If there's not already a sunk_object, this fgObj will sink
void River::try_to_sink(FgObj& fgObj) {
  if (!sunk_object) {
    if (dynamic_cast<Barrel*>(&fgObj) != nullptr) {
      iscontaminated = 2;
      current_grid->grid_environment_score -= 5;
      // delete this->water->transform;
      // *water = (model_loader->create_model("Water_Toxic"));
      // water->transform->position = this->drawable->transform->position;
      set_purple_amt(1);
    }
    just_sunk = true;
  }
}


bool check_contaminated(int x, int y){
    if (x<0 || (size_t)x>= current_grid->width || y<0 || (size_t)y>= current_grid->height) return false;
    River* r = dynamic_cast<River*>(current_grid->cells[x][y].bgTile);
    if (r != nullptr && r->iscontaminated==2) return true;
    return false;
}


void River::on_post_tick(){
	if(just_sunk) {
		just_sunk = false; 
		sunk_object = this->cell->fgObj->drawable;
		delete this->cell->fgObj; 
		this->cell->fgObj = nullptr; 
		sunk_object->transform->position.z -= 0.25; 
	}
    if (iscontaminated || willbecontaminated) {
        return;
    }
    if (check_contaminated(this->cell->pos.x-1, this->cell->pos.y)||
            check_contaminated(this->cell->pos.x+1, this->cell->pos.y)||
            check_contaminated(this->cell->pos.x, this->cell->pos.y-1)||
            check_contaminated(this->cell->pos.x, this->cell->pos.y+1)){
        willbecontaminated = true;
    }
}


void River::on_pre_tick(){
    if (willbecontaminated||iscontaminated==1) contaminated();
}


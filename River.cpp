#include "River.hpp"
#include "Barrel.hpp"
#include "Player.hpp"
#include "LitToxicColorTextureProgram.hpp"

#include <iostream>
#include "Ramp.hpp"


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
  //extra_drawables.push_back(water);
  this->water->pipeline.set_uniforms = []() {
    glUniform1f(lit_toxic_color_texture_program->BROWN_AMT_float, .05f);
  };

  if (sunk_object_model_name.length() > 0) {
    scene->drawables.push_back(model_loader->create_model(sunk_object_model_name));
    sunk_object = &(scene->drawables.back());
  }
}


// Set the transforms of all drawables appropriately
void River::position_models() {
  //position the layered models
  CellItem::position_models();
  water->transform->position = this->drawable->transform->position;
  apply_purple_amt();
  if (sunk_object != nullptr) {
    sunk_object->transform->position = this->drawable->transform->position;
    sunk_object->transform->position.z -= 0.25;
  }
}


// Create a copy with no drawables
River* River::clone_lightweight(Cell* new_cell) {
  River* new_river = new River(*this);
  new_river->cell = new_cell;
  new_river->drawable = nullptr;
  new_river->water = nullptr;
  new_river->sunk_object = nullptr;
  new_river->extra_drawables.clear();
  return new_river;
}


// Applies the current purple amount of the river to its uniforms, based on iscontaminated
void River::apply_purple_amt() {
  float amt = (float)is_contaminated / (float)max_contaminated;
  //if (amt > 0) {
    this->water->pipeline.set_uniforms = [amt]() {
      glUniform1f(lit_toxic_color_texture_program->PURPLE_AMT_float, amt);
      glUniform1f(lit_toxic_color_texture_program->BROWN_AMT_float, .05f);

    };
  //}
}


void River::contaminated() {
    current_grid->grid_environment_score -= 5;
    is_contaminated += 1;
    if (is_contaminated == 2) {
      AudioManager::clips_to_play.push(AudioManager::AudioClip::TOXIN_SPREAD);
    }
    will_be_contaminated = false;
    // delete this->water->transform;
    // *water = (model_loader->create_model("Water_Toxic"));
    // water->transform->position = this->drawable->transform->position;
    apply_purple_amt();
}


bool River::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  if (!sunk_object && dynamic_cast<Player*>(&objBeingMoved) != nullptr) return false;
  if (!sunk_object && dynamic_cast<Ramp*>(&objBeingMoved) != nullptr) return false;
  return BgTile::can_fg_obj_move_into(objBeingMoved, displ);
}


void River::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
  try_to_sink(objBeingMoved);
}


// If there's not already a sunk_object, this fgObj will sink
void River::try_to_sink(FgObj& fgObj) {
  if (!sunk_object) {
    if (dynamic_cast<Barrel*>(&fgObj) != nullptr) {
      is_contaminated = 2;
      AudioManager::clips_to_play.push(AudioManager::AudioClip::TOXIN_SPREAD);
      current_grid->grid_environment_score -= 5;
      // delete this->water->transform;
      // *water = (model_loader->create_model("Water_Toxic"));
      // water->transform->position = this->drawable->transform->position;
      apply_purple_amt();
    }
    just_sunk = true;
  }
}


bool River::check_contaminated(int x, int y){
    glm::ivec2 pos = glm::ivec2(x, y);
    if (!current_grid->is_valid_pos(pos)) return false;
    River* r = dynamic_cast<River*>(current_grid->cell_at(pos)->bgTile);
    if (r != nullptr && r->is_contaminated == r->max_contaminated) return true;
    return false;
}


void River::on_post_tick(){
	if(just_sunk) {
		just_sunk = false; 
		sunk_object = this->cell->fgObj->drawable;
    sunk_object_model_name = this->cell->fgObj->get_model_name();
		delete this->cell->fgObj; 
		this->cell->fgObj = nullptr; 
		sunk_object->transform->position.z -= 0.25;
	}
    if (is_contaminated > 0 || will_be_contaminated) {
        return;
    }
    if (check_contaminated(this->cell->pos.x-1, this->cell->pos.y)||
            check_contaminated(this->cell->pos.x+1, this->cell->pos.y)||
            check_contaminated(this->cell->pos.x, this->cell->pos.y-1)||
            check_contaminated(this->cell->pos.x, this->cell->pos.y+1)){
      will_be_contaminated = true;
    }
}


void River::on_pre_tick(){
    if (will_be_contaminated || (is_contaminated > 0 && is_contaminated < max_contaminated)) contaminated();
}


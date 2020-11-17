#include "River.hpp"
#include "Barrel.hpp"
#include "Player.hpp"
#include <iostream>



void River::contaminated() {
    current_grid->environment_score -= 5;
    iscontaminated = true;
    willbecontaminated = false;
	this->rotten.transform->position = this->drawable->transform->position;
    delete this->water->transform;
    *water = this->rotten;
}

bool River::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  if (!sunk_object && dynamic_cast<Player*>(&objBeingMoved) != nullptr) return false;
  return BgTile::can_fg_obj_move_into(objBeingMoved, displ);
}

void River::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
	if(!sunk_object) {
		if(dynamic_cast<Barrel*>(&objBeingMoved) != nullptr) {
			iscontaminated = true;
			current_grid->environment_score -= 5;
			this->rotten.transform->position = this->drawable->transform->position;
			delete this->water->transform;
			*water = this->rotten;
		}
		just_sunk = true; 
	}
}

void River::set_position_model(Scene::Drawable* _water){
    this->water = _water;
}

void River::position_models() {
    //position the layered models
	if(water->transform)
		water->transform->position = this->drawable->transform->position;
}

bool check_contaminated(int x, int y){
    if (x<0 || (size_t)x>= current_grid->width || y<0 || (size_t)y>= current_grid->height) return false;
    River* r = dynamic_cast<River*>(current_grid->cells[x][y].bgTile);
    if (r != nullptr && r->iscontaminated) return true;
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
    if (willbecontaminated) contaminated();
}


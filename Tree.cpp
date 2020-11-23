#include "Tree.hpp"
#include <iostream>


// Fixed rock can't be displaced.
bool Tree::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  return false;
}

void Tree::on_post_tick() {
	int flower_state = current_grid->tree_flower_states[id];
	if (flower_state == 0) {
		this->tree1.transform = this->drawable->transform;
		*(this->drawable) = this->tree1;
	} else if (flower_state == 1) {
		this->tree2.transform = this->drawable->transform;
		*(this->drawable) = this->tree2;
	} else if (flower_state == 2) {
		this->tree3.transform = this->drawable->transform;
		*(this->drawable) = this->tree3;
	}
}

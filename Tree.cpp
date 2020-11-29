#include "Tree.hpp"
#include <iostream>


// Load any drawables
void Tree::load_models(Scene* scene) {
	scene->drawables.push_back(model_loader->create_model("Tree"));
	this->drawable = &(scene->drawables.back());

	scene->drawables.push_back(model_loader->create_model("Tree_Flower1"));
	tree2 = &(scene->drawables.back());
	this->extra_drawables.push_back(tree2);

	scene->drawables.push_back(model_loader->create_model("Tree_Flower2"));
	tree3 = &(scene->drawables.back());
	this->extra_drawables.push_back(tree3);
}


// Create a copy with no drawables
Tree* Tree::clone_lightweight(Cell* new_cell) {
	Tree* new_tree = new Tree(*this);
	new_tree->cell = new_cell;
	new_tree->drawable = nullptr;
	new_tree->tree2 = nullptr;
	new_tree->tree3 = nullptr;
	new_tree->extra_drawables.clear();
	return new_tree;
}


// Tree can't be displaced.
bool Tree::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  return false;
}


// Called after input is handled. Cycles the tree animation
void Tree::on_post_tick() {
	// Odds to progress the flower state
	if (rand() % 100 < flower_prob[flower_state]) {
		flower_state = (flower_state + 1) % flower_prob.size();
	}

	// Update drawables
	this->drawable->disabled = (flower_state != 0);
	this->tree2->disabled = (flower_state != 1);
	this->tree3->disabled = (flower_state != 2);
}

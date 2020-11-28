#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"


struct Tree : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;
  Tree(Scene *scene); 

	// Fields
	Scene::Drawable tree1 = nullptr;
	Scene::Drawable tree2 = nullptr;
	Scene::Drawable tree3 = nullptr;

	unsigned int flower_state = 0;

	const std::vector<int> flower_prob { 50, 20, 20 };

	// Methods
	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;

	void on_post_tick() override;
};
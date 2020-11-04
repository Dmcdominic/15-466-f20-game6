#pragma once

#include "Grid.hpp"
#include "Scene.hpp"


struct Barrel : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Fields
	static Scene::Drawable* prefab;

	bool rollable_up_down = true; // False = rollable left/right

	// Methods
	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;

	//void on_post_tick() override;

	void rotate_90();
};
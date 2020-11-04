#pragma once

#include "Grid.hpp"
#include "Scene.hpp"


struct Barrel : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Fields
	static Scene::Drawable* prefab;

	// Methods
	// TODO
	/*
	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	*/
};
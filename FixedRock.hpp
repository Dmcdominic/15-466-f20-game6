#pragma once

#include "Grid.hpp"
#include "Scene.hpp"


struct FixedRock : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Fields
	static Scene::Drawable* prefab;

	// Methods
	bool can_fg_obj_move_into(const FgObj& objBeingMoved, const glm::ivec2& displ) override;
};
#pragma once

#include "Grid.hpp"
#include "Scene.hpp"


struct Ramp : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Methods
	// TODO
	void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
};
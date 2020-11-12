#pragma once

#include "Grid.hpp"


struct Barrel : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Fields
	bool rollable_up_down = true; // False = rollable left/right

	// Methods
	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;

	void rotate_90() override;
};
#pragma once

#include "Grid.hpp"

struct Player : FgObj {
	bool can_fg_obj_move_into(const FgObj& objBeingMoved, const glm::ivec2& displ) override;
	void when_fg_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;

	bool on_input(/* TODO - add some kind of input type here */) override;
};
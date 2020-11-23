#pragma once

#include <optional>

#include "Grid.hpp"
#include "Scene.hpp"


struct Player : FgObj {
	// Fields
	std::optional<glm::ivec2> next_forced_move;

	// Constructors (inherited)
	using FgObj::FgObj;

	// Methods
	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;

	bool can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ) override;
	void when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ) override;

	bool on_input(const Input& input, Output* output) override;
};
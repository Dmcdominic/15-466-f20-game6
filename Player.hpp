#pragma once

#include "Grid.hpp"
#include "Scene.hpp"


struct Player : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Fields
	static Scene::Drawable* prefab;

	// Methods
	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;

	bool can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ) override;
	void when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ) override;

	bool on_input(const Input& input) override;
};
#pragma once

#include <optional>

#include "Grid.hpp"
#include "Scene.hpp"


struct Player : FgObj {
	// Fields
	std::optional<glm::ivec2> next_forced_move;

	// Constructors (inherited)
	using FgObj::FgObj;

	// Fields
	Scene::Drawable idle0 = nullptr;
	Scene::Drawable idle1 = nullptr;
	size_t idle_num = 40;
	size_t idle_counter = 0;
	bool player_state = 0;

	Player(Scene::Drawable *_drawable, Scene::Drawable _idle0, Scene::Drawable _idle1):
			FgObj(_drawable), idle0(_idle0), idle1(_idle1){};

	// Methods
	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;

	bool can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ) override;
	void when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ) override;

	bool on_input(const Input& input, Output* output) override;
};
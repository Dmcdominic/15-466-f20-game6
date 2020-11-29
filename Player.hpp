#pragma once

#include <optional>

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"


struct Player : FgObj {
	// Fields
	std::optional<glm::ivec2> next_forced_move = std::nullopt;

	Scene::Drawable *idle0 = nullptr;
	const size_t idle_num = 40;
	size_t idle_counter = 0;
	bool player_state = false;

	// Constructors (inherited)
	using FgObj::FgObj;

	// Methods
	void load_models(Scene* scene) override;
	Player* clone_lightweight(Cell* new_cell) override;

	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;

	bool on_input(const Input& input, Output* output) override;
	void on_update();
};
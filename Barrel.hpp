#pragma once

#include "Grid.hpp"
#include "ModelLoader.hpp"


struct Barrel : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;
	Barrel(Scene* scene, int _rotations = 0);

	// Fields
	float displ_step = 0.1f;
	float displ_amt = 0.0f;
	bool rolling = false;
	int saved_rotations = 0;
	glm::vec2 saved_displ = glm::vec2(0, 0);
	glm::vec2 prev_pos = glm::vec2(0, 0);
	Cell* saved_target_cell = nullptr;

	// Methods
	std::string get_model_name() override { return "Barrel"; }

	void load_models(Scene* scene) override;
	Barrel* clone_lightweight(Cell* new_cell) override;

	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	std::optional<AudioManager::AudioClip> get_move_clip() override;

	bool rollable_up_down();

	// for rolling
	void roll();
	void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	bool try_to_move_by(const glm::ivec2 &displ) override;

	static bool roll_rolling_barrels(Grid *grid);
};

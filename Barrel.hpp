#pragma once

#include "Grid.hpp"
#include "ModelLoader.hpp"


struct Barrel : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;
	Barrel(Scene* scene, int _rotations = 0);

	// Methods
	void load_models(Scene* scene) override;
	Barrel* clone_lightweight(Cell* new_cell) override;

	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	std::optional<AudioManager::AudioClip> get_move_clip() override;

	bool rollable_up_down();
};

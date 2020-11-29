#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"


struct Rock : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Methods
	void load_models(Scene* scene) override;
	Rock* clone_lightweight(Cell* new_cell) override;

	std::optional<AudioManager::AudioClip> get_move_clip() override;
};
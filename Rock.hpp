#pragma once

#include "Grid.hpp"
#include "Scene.hpp"


struct Rock : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Methods
	std::optional<AudioManager::AudioClip> get_move_clip() override;
};
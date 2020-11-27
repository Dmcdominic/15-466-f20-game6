#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"


struct Ramp : FgObj {
	// Fields
	glm::ivec2 up_dir = glm::ivec2(0, 1);

	// Constructors (inherited)
	using FgObj::FgObj;
	Ramp(Scene *scene);

	// Methods
	void rotate_90() override;

	bool should_slide_up(const glm::ivec2& displ);
};
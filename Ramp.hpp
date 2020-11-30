#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"


struct Ramp : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Methods
	std::string get_model_name() override { return "Ramp"; }

	void load_models(Scene* scene) override;
	Ramp* clone_lightweight(Cell* new_cell) override;

	bool should_slide_up(const glm::ivec2& displ);
};
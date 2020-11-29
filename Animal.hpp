#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"


struct Animal : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Methods
	void load_models(Scene* scene) override;
	Animal* clone_lightweight(Cell* new_cell) override;
};
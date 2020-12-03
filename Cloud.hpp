#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"


struct Cloud : SkyObj {
	// Constructor
	Cloud(Scene* scene);

		// Methods
		void load_models(Scene* scene) override;
		Cloud* clone_lightweight(Cell* new_cell) override;
};
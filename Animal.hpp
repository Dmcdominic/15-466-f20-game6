#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"


struct Animal : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;
    Scene::Drawable* dead = nullptr;
	bool is_dead = false; 

	// Methods
	std::string get_model_name() override { return "Animal"; }

	void load_models(Scene* scene) override;
	Animal* clone_lightweight(Cell* new_cell) override;
	void on_post_tick() override;
};
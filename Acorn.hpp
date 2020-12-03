#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"


struct Acorn : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Fields
	bool sprouted = false; 
  	Scene::Drawable *sapling = nullptr;

	// Methods
	std::string get_model_name() override { return "Acorn"; }

	void load_models(Scene* scene) override;
	void sprout(); 
	Acorn* clone_lightweight(Cell* new_cell) override;
	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;

	std::optional<AudioManager::AudioClip> get_move_clip() override;
};
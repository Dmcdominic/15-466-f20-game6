#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"

struct River : BgTile {
	// Fields
	bool left, right, upper, lower;

	int iscontaminated = 0;
	bool willbecontaminated = false;
	bool just_sunk = false;

	Scene::Drawable* water = nullptr;
	Scene::Drawable* sunk_object = nullptr;

	std::vector<River*> *tiles = nullptr;

	// Constructor
	River(Scene *scene, bool left, bool right, bool upper, bool lower);

	// Methods
	void load_models(Scene *scene) override;
	void position_models() override;
	River* clone_lightweight(Cell* new_cell) override;

	void set_purple_amt(float amt);
	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	void try_to_sink(FgObj& fgObj);
	void contaminated();
	void on_post_tick() override;
	void on_pre_tick() override;
};

#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"

struct River : BgTile {
	// Fields
	bool left, right, upper, lower;

	const int max_contaminated = 2;
	int is_contaminated = 0;
	bool will_be_contaminated = false;
	bool just_sunk = false;

	Scene::Drawable* water = nullptr;
	Scene::Drawable* sunk_object = nullptr;
	glm::quat sunk_obj_rotation = glm::quat();
	std::string sunk_object_model_name = "";


	// Constructor
	River(Scene *scene, bool left, bool right, bool upper, bool lower);


	// Methods
	void load_models(Scene *scene) override;
	void position_models() override;
	River* clone_lightweight(Cell* new_cell) override;

	void make_lightweight();

	void apply_purple_amt();

	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	void try_to_sink(FgObj& fgObj);
	
	void contaminated();
	static bool check_contaminated(int x, int y);

	void on_post_tick() override;
	void on_pre_tick() override;
};

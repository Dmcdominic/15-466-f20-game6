#pragma once

#include "Grid.hpp"
#include "Scene.hpp"

struct River : BgTile {
		// Constructors (inherited)

		// Fields
		// static Scene::Drawable* prefab;
        bool iscontaminated = false;
        bool willbecontaminated = false;
		Scene::Drawable rotten;
		Scene::Drawable* water = nullptr;
		std::list<Scene::Drawable> *drawables = nullptr;

		std::vector<River*> *tiles = nullptr;

		// Methods
		River(Scene::Drawable* _drawable, Scene::Drawable _rotten, std::list<Scene::Drawable>* _drawables): BgTile(_drawable),
		rotten(_rotten), drawables(_drawables){};

		bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
		void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
		void contaminated();
		void set_position_model(Scene::Drawable* _water);
		void position_models();
        void on_post_tick() override;
        void on_pre_tick() override;
};

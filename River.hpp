#pragma once

#include "Grid.hpp"
#include "Scene.hpp"

struct River : BgTile {
		// Constructors (inherited)

		// Fields
		static Scene::Drawable* prefab;
		Scene::Drawable rotten = nullptr;
		std::list<Scene::Drawable> *drawables = nullptr;

		std::vector<River*> *tiles = nullptr;

		// Methods
		River(Scene::Drawable* _drawable, Scene::Drawable _rotten, std::list<Scene::Drawable>* _drawables): BgTile(_drawable),
		rotten(_rotten), drawables(_drawables){};

		bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
		void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
		void contaminated();

};

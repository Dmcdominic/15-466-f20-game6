#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"

struct River : BgTile {
		// Constructors (inherited)

		// Fields
        int iscontaminated = 0;
        bool willbecontaminated = false;
		bool just_sunk = false; 
		//Scene::Drawable* toxic_water;
		Scene::Drawable* water = nullptr;
		Scene::Drawable* sunk_object = nullptr;
		std::list<Scene::Drawable> *drawables = nullptr;

		std::vector<River*> *tiles = nullptr;

		// Methods
		//River(Scene::Drawable* _drawable, Scene::Drawable *_toxic_water, std::list<Scene::Drawable>* _drawables): BgTile(_drawable), toxic_water(_toxic_water), drawables(_drawables){};
  		River(Scene::Drawable* _drawable, std::list<Scene::Drawable>* _drawables);

		void set_purple_amt(float amt);
	  	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
		void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
		void contaminated();
		void position_models();
        void on_post_tick() override;
        void on_pre_tick() override;
};

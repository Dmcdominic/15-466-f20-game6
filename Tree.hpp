#pragma once

#include "Grid.hpp"
#include "Scene.hpp"


struct Tree : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Fields
	static Scene::Drawable *prefab;
	Scene::Drawable tree1 = nullptr;
	Scene::Drawable tree2 = nullptr;
	Scene::Drawable tree3 = nullptr;
	int id;

	// Methods
	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;

	Tree(Scene::Drawable *_drawable, Scene::Drawable _tree1, Scene::Drawable _tree2, Scene::Drawable _tree3, int _id):
			FgObj(_drawable), tree1(_tree1), tree2(_tree2), tree3(_tree3), id(_id){};
	void on_post_tick() override;
};
#pragma once
#include "Grid.hpp"
#include "PackedGrid.hpp"
#include "ModelLoader.hpp"

struct GridLoader {
	static Grid* load_level(unsigned int level_num, Scene *scene);
	static Grid* create_undo_copy(Grid *src);
	static void load_undo_copy(Grid* undo_copy, Scene* scene);
};
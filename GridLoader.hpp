#pragma once
#include "Grid.hpp"
#include "PackedGrid.hpp"
#include "ModelLoader.hpp"

struct GridLoader {
	static Grid* load_level(unsigned int level_num, Scene *scene);
	static Grid* create_undo_copy();
};
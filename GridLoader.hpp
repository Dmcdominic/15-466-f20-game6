#pragma once
#include "Grid.hpp"
#include "PackedGrid.hpp"
#include "ModelLoader.hpp"

struct GridLoader {
	static Grid* load_level(unsigned int level_num, ModelLoader loader, Scene *scene);
};
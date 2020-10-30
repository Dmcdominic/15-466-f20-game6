#include "Grid.hpp"
#include "Level.hpp"
#include "PackedGridLayer.hpp"


struct LevelLoader {
	static Level load_level(unsigned int level_num);
};
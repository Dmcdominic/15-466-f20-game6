#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"


struct Cloud : SkyObj {
	// Constructors (inherited)
	using SkyObj::SkyObj;
  	Cloud(Scene *scene); 

};
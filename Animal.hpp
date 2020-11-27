#pragma once

#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"


struct Animal : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;
  	Animal(Scene *scene); 

};
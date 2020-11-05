#pragma once

#include "Grid.hpp"
#include "Scene.hpp"


struct Rock : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Fields
	static Scene::Drawable* prefab;

	// Methods
	// No need to override anything. Pushable is default FgObj behavior.
};
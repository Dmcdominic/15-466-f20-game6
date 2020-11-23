#pragma once

#include "Grid.hpp"
#include "Scene.hpp"


struct Rock : FgObj {
	// Constructors (inherited)
	using FgObj::FgObj;

	// Methods
	// No need to override anything. Pushable is default FgObj behavior.
};
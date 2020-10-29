#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Scene.hpp"


// Forward declarations
struct Grid;
struct Cell;
struct BgTile;
struct FgObj;
struct SkyObj;


// The current grid
extern Grid* current_grid;


/* A grid of cells. */
struct Grid {
	// Fields
	std::vector<std::vector<Cell>> cells;

	// Constructor
	Grid(int width, int height);
	// Destructor
	~Grid();
};


/* A Cell is a single square in the puzzle grid.
 * It contains its position in the grid, 1 bgTile, and 1 fgObj.
 */
struct Cell {
	// Fields
	glm::ivec2 pos = glm::ivec2();
	BgTile* bgTile = nullptr;
	FgObj* fgObj = nullptr;
	SkyObj* skyObj = nullptr;

	// Constructor
	Cell(glm::ivec2 _pos);
	// Destructor
	~Cell();

	// Methods
	bool can_fg_obj_move_into(const FgObj& objBeingMoved, const glm::ivec2 &displ);
	void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ);

	bool can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ);
	void when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ);

	bool on_input(/* TODO - add some kind of input type here */);
};


/* Background Tiles */
struct BgTile {
	Cell* cell = nullptr;
	BgTile(Cell* _cell) : cell(_cell) {};

	virtual bool can_fg_obj_move_onto(const FgObj& objBeingMoved, const glm::ivec2& displ);
	virtual void when_fg_obj_moved_onto(FgObj& objBeingMoved, const glm::ivec2& displ);

	bool can_sky_obj_move_onto(const SkyObj& objBeingMoved, const glm::ivec2& displ);
	void when_sky_obj_moved_onto(SkyObj& objBeingMoved, const glm::ivec2& displ);

	virtual bool on_input(/* TODO - add some kind of input type here */);
};


/* Foreground Objects */
struct FgObj {
	Cell* cell = nullptr;
	Scene::Transform *transform = nullptr;
	FgObj(Cell* _cell, Scene::Transform* _transform) : cell(_cell), transform(_transform) {};

	virtual bool can_fg_obj_move_into(const FgObj& objBeingMoved, const glm::ivec2& displ);
	virtual void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ);

	bool can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ);
	void when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ);

	virtual bool on_input(/* TODO - add some kind of input type here */);
};


/* Sky Objects */
struct SkyObj {
	Cell* cell = nullptr;
	Scene::Transform* transform = nullptr;
	SkyObj(Cell* _cell, Scene::Transform* _transform) : cell(_cell), transform(_transform) {};

	virtual bool can_fg_obj_move_into(const FgObj& objBeingMoved, const glm::ivec2& displ);
	virtual void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ);

	bool can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ);
	void when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ);

	virtual bool on_input(/* TODO - add some kind of input type here */);
};
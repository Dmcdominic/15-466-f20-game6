#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <stack>

#include "Scene.hpp"
#include "Input.hpp"
#include "AudioManager.hpp"


// Forward declarations
struct Grid;
struct Cell;
struct BgTile;
struct FgObj;
struct SkyObj;
struct Player;
struct OverworldNode;


// The current grid
extern Grid* current_grid;

// The undo stack
extern std::stack<Grid*> undo_grids;


/* A grid of cells. */
struct Grid {
	// Fields
	size_t width = 0;
	size_t height = 0;
	unsigned int goal; 
	unsigned int num_disposed;
	std::vector<std::vector<Cell>> cells;

	int grid_environment_score = 0;

	Player *player = nullptr;
	OverworldNode *highest_level_node = nullptr;

	// Constructor
	Grid(size_t _width, size_t _height, unsigned int _goal, unsigned int _num_disposed);
	// Destructor
	~Grid();

	// Methods
	bool is_valid_pos(glm::ivec2 _pos);
	Cell* cell_at(glm::ivec2 _pos);
	bool on_input(const Input& input, Output* output);

	static glm::ivec2 normalize_displ(const glm::ivec2& displ);
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
	void set_bg_tile(BgTile* _bgTile);
	void set_fg_obj(FgObj* _fgObj);
	void set_sky_obj(SkyObj* _skyObj);

	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2 &displ);
	void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ);

	bool can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ);
	void when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ);

	bool on_input(const Input &input, Output* output);
	void on_pre_tick();
	void on_post_tick();
};



/* Cell Items (parent class of BgTile, FgObj, SkyObj, etc.) */
struct CellItem {
	// Fields
	Scene::Drawable* drawable = nullptr;
	std::vector<Scene::Drawable*> extra_drawables = std::vector<Scene::Drawable*>();
	Cell* cell = nullptr;
	int rotations = 0;

	// Constructors
	CellItem() {};
	CellItem(Scene *scene, int _rotations = 0);

	// Methods
	void load_and_reposition_models(Scene *scene);

	virtual void load_models(Scene* scene) = 0;
	virtual void position_models();
	virtual CellItem* clone_lightweight(Cell* new_cell) = 0; // Create a copy with no drawables

	virtual bool try_to_move_by(const glm::ivec2& displ) = 0;

	virtual bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) = 0;
	virtual void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) = 0;

	virtual bool can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ) = 0;
	virtual void when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ) = 0;

	virtual bool on_input(const Input& input, Output *output) = 0;
	virtual void on_pre_tick() {};
	virtual void on_post_tick() {};

	virtual void rotate_90(bool skip_incr = false);

	virtual std::optional<AudioManager::AudioClip> get_move_clip();
	void push_move_clip();
};



/* Background Tiles */
struct BgTile : CellItem {
	// Constructors (inherited)
	using CellItem::CellItem;
	// Destructor
	virtual ~BgTile() {};

	// Methods
	virtual BgTile* clone_lightweight(Cell* new_cell) override = 0;

	bool try_to_move_by(const glm::ivec2& displ) override;

	virtual bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	virtual void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;

	virtual bool can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ) override;
	virtual void when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ) override;

	virtual bool on_input(const Input& input, Output* output) override;
};



/* Foreground Objects */
struct FgObj : CellItem {
	// Constructors (inherited)
	using CellItem::CellItem;
	// Destructor
	virtual ~FgObj() {};

	// Methods
	virtual FgObj* clone_lightweight(Cell* new_cell) override = 0;

	bool try_to_move_by(const glm::ivec2& displ) override;

	virtual bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	virtual void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;

	virtual bool can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ) override;
	virtual void when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ) override;

	virtual bool on_input(const Input& input, Output* output) override;
};



/* Sky Objects */
struct SkyObj : CellItem {
	// Constructors (inherited)
	using CellItem::CellItem;
	// Destructor
	virtual ~SkyObj() {};

	// Methods
	virtual SkyObj* clone_lightweight(Cell* new_cell) override = 0;

	bool try_to_move_by(const glm::ivec2& displ) override;

	virtual bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	virtual void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;

	virtual bool can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ) override;
	virtual void when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ) override;

	virtual bool on_input(const Input& input, Output* output) override;
};

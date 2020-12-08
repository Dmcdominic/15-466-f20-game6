#include "Grid.hpp"
#include <stdexcept>
#include <iostream>

#include "Load.hpp"
#include "data_path.hpp"


// Variable declarations
Grid* current_grid = nullptr;
std::stack<Grid*> undo_grids = std::stack<Grid*>();




/* ----- Grid ----- */

// Grid constructor
Grid::Grid(size_t _w, size_t _h, unsigned int _g, unsigned int _num_disposed, int _env_score) : width(_w), height(_h), goal(_g), num_disposed(_num_disposed), grid_environment_score(_env_score) {
  cells = std::vector<std::vector<Cell>>(width, std::vector<Cell>(height, Cell(glm::ivec2(0))));
  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      cells[x][y].pos = glm::ivec2(x, y);
    }
  }
}


// Grid destructor
Grid::~Grid() {
  cells.clear();
}


// Returns true iff x and y are within the bounds of the grid
bool Grid::is_valid_pos(glm::ivec2 _pos) {
  return (_pos.x >= 0 && (size_t)_pos.x < this->width &&
          _pos.y >= 0 && (size_t)_pos.y < this->height);
}


// Returns the cell at the given position.
Cell* Grid::cell_at(glm::ivec2 _pos) {
  assert(is_valid_pos(_pos));
  return &cells.at(_pos.x).at(_pos.y);
}


// Calls the on_input function on every cell in the grid.
// Returns true iff something handled the input.
// For now, it only allows 1 CellItem to handle any input.
bool Grid::on_input(const Input& input, Output* output) {
  pre_tick_done = false;
  // pre_tick() now must be called within every on_input() before returning true

  // on_input()
  bool input_handled = false;
  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      if (cells[x][y].on_input(input, output)) {
        input_handled = true;
        x = width;
        y = height;
      }
    }
  }

  // If it was an invalid move or other input that couldn't be handled,
  //   play the error sound.
  if (!input_handled) {
    AudioManager::clips_to_play.push(AudioManager::AudioClip::ERROR);
    return false;
  }

  // If post_tick is queued, it will be handled later by PlayMode
  if (!current_grid->post_tick_queued) {
    post_tick();
  }

  return true;
}


// Call this to run on_pre_tick() for all cells.
//   But will only run it once per on_input()
void Grid::pre_tick() {
  if (pre_tick_done) return;
  pre_tick_done = true;
  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      cells[x][y].on_pre_tick();
    }
  }
}


// Call this to run on_post_tick() for all cells
void Grid::post_tick() {
  current_grid->post_tick_queued = false;
  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      cells[x][y].on_post_tick();
    }
  }
}


// Returns a copy of a displacement vector that is "normalized":
//   Positive coordinates set to  1
//   Negative coordinates set to -1
//   Otherwise set to 0
glm::ivec2 Grid::normalize_displ(const glm::ivec2& displ) {
  int x = (displ.x == 0) ? 0 : ((displ.x < 0) ? -1 : 1);
  int y = (displ.y == 0) ? 0 : ((displ.y < 0) ? -1 : 1);
  return glm::ivec2(x, y);
}



/* ----- Cell ----- */

// Cell constructor
Cell::Cell(glm::ivec2 _pos) : pos(_pos) {
};


// Cell destructor
Cell::~Cell() {
  // TODO - implement these destructors?
  if (bgTile != nullptr) delete bgTile;
  if (fgObj != nullptr) delete fgObj;
  if (skyObj != nullptr) delete skyObj;
}


// --- Setters ---
/* Each Cell setter for bgTile/fgObj/skyObj does all of the following:
*    - Checks that you're not setting a new tile/object on a cell that already has one (you should handle that one first).
*    - Sets this Cell's corresponding tile/obj field.
*  If the given tile/obj is not null:
*    - If its current cell is not null, sets THAT Cell's corresponding tile/obj field to null.
*    - Sets the given tile/obj's cell field to this Cell.
*    - Moves the tile/obj's transform to this Cell's position.
*/

// See above
void Cell::set_bg_tile(BgTile* _bgTile) {
  if (this->bgTile != nullptr && _bgTile != nullptr) throw std::runtime_error("Setting the bgTile of a cell that already has one.");
  this->bgTile = _bgTile;
  if (_bgTile == nullptr) return;

  if (_bgTile->cell != nullptr) {
    _bgTile->cell->bgTile = nullptr;
  }
  this->bgTile->cell = this;
  if (this->bgTile->drawable == nullptr) return;
  // smoothly move/animate this object?
  if (!this->bgTile->drawable->transform) throw std::runtime_error("No transform on a BgTile that's trying to move");
  this->bgTile->position_models();
}


// See above
void Cell::set_fg_obj(FgObj* _fgObj) {
  if (this->fgObj != nullptr && _fgObj != nullptr) throw std::runtime_error("Setting the fgObj of a cell that already has one.");
  this->fgObj = _fgObj;
  if (_fgObj == nullptr) return;
  
  if (_fgObj->cell != nullptr) {
    _fgObj->cell->fgObj = nullptr;
  }
  this->fgObj->cell = this;
  if (this->fgObj->drawable == nullptr) return;
  // smoothly move/animate this object?
  if (!this->fgObj->drawable->transform) throw std::runtime_error("No transform on a FgObj that's trying to move");
  this->fgObj->position_models();
}


// See above
void Cell::set_sky_obj(SkyObj* _skyObj) {
  if (this->skyObj != nullptr && _skyObj != nullptr) throw std::runtime_error("Setting the skyObj of a cell that already has one.");
  this->skyObj = _skyObj;
  if (_skyObj == nullptr) return;

  if (_skyObj->cell != nullptr) {
    _skyObj->cell->skyObj = nullptr;
  }
  this->skyObj->cell = this;
  if (this->skyObj->drawable == nullptr) return;
  // smoothly move/animate this object?
  if (!this->skyObj->drawable->transform) throw std::runtime_error("No transform on a SkyObj that's trying to move");
  this->skyObj->position_models();
}


// Returns true iff a foreground object can safely be moved/pushed into this cell
bool Cell::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  return (bgTile == nullptr || bgTile->can_fg_obj_move_into(objBeingMoved, displ)) &&
         (fgObj == nullptr  || fgObj->can_fg_obj_move_into(objBeingMoved, displ)) &&
         (skyObj == nullptr || skyObj->can_fg_obj_move_into(objBeingMoved, displ));
}


// Does whatever should happen when the given foreground object is moved/pushed into this cell.
// displ is a vector representing the displacement of the object being moved.
void Cell::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  if (bgTile != nullptr) {
    bgTile->when_fg_obj_moved_into(objBeingMoved, displ);
  }
  if (fgObj != nullptr) {
    fgObj->when_fg_obj_moved_into(objBeingMoved, displ);
  }
  if (skyObj != nullptr) {
    skyObj->when_fg_obj_moved_into(objBeingMoved, displ);
  }
}


// Returns true iff a sky object can safely be moved/pushed into this cell
bool Cell::can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ) {
  return (bgTile == nullptr || bgTile->can_sky_obj_move_into(objBeingMoved, displ)) &&
         (fgObj == nullptr  || fgObj->can_sky_obj_move_into(objBeingMoved, displ)) &&
         (skyObj == nullptr || skyObj->can_sky_obj_move_into(objBeingMoved, displ));
}


// Does whatever should happen when the given foreground object is moved/pushed into this cell.
// displ is a vector representing the displacement of the object being moved.
void Cell::when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ) {
  if (bgTile != nullptr) {
    bgTile->when_sky_obj_moved_into(objBeingMoved, displ);
  }
  if (fgObj != nullptr) {
    fgObj->when_sky_obj_moved_into(objBeingMoved, displ);
  }
  if (skyObj != nullptr) {
    skyObj->when_sky_obj_moved_into(objBeingMoved, displ);
  }
}


// When there is some input, pass the input to each tile/obj in this cell.
// Returns true iff any of the tiles/objects return true.
bool Cell::on_input(const Input& input, Output* output) {
  return (bgTile != nullptr && bgTile->on_input(input, output)) |
    (fgObj != nullptr && fgObj->on_input(input, output)) |
    (skyObj != nullptr && skyObj->on_input(input, output));
}


// Called right before on_input() for all cells.
void Cell::on_pre_tick() {
  if (bgTile != nullptr) bgTile->on_pre_tick();
  if (fgObj != nullptr) fgObj->on_pre_tick();
  if (skyObj != nullptr) skyObj->on_pre_tick();
}


// Called right after on_input() for all cells.
void Cell::on_post_tick() {
  if (bgTile != nullptr) bgTile->on_post_tick();
  if (fgObj != nullptr) fgObj->on_post_tick();
  if (skyObj != nullptr) skyObj->on_post_tick();
}



/* ----- Cell Items ----- */

// Constructor
CellItem::CellItem(Scene* scene, int _rotations) : rotations(_rotations) {
 //load_and_reposition_models(scene);
}


// Destructor
CellItem::~CellItem() {
  //TODO - Remove drawables? Currently this is just being done by clearing drawables
}


// Load the models, position them, and rotate an appropriate number of times.
void CellItem::load_and_reposition_models(Scene* scene) {
  load_models(scene);
  position_models();
  for (int i = 0; i < rotations; i++) {
    rotate_90(true);
  }
}


// Set the transforms of all drawables appropriately
void CellItem::position_models() {
  this->drawable->transform->position = glm::vec3(this->cell->pos.x, this->cell->pos.y, 0.0f);
  for (auto drawable_iter = extra_drawables.begin(); drawable_iter != extra_drawables.end(); drawable_iter++) {
    (*drawable_iter)->transform->position = this->drawable->transform->position;
    (*drawable_iter)->transform->rotation = this->drawable->transform->rotation;
  }
}


// Rotates the tile/object 90 degrees (clockwise for now)
void CellItem::rotate_90(bool skip_incr) {
  if (!skip_incr) rotations = (rotations + 1) % 4;
  this->drawable->transform->rotate_90();
  for (auto drawable_iter = extra_drawables.begin(); drawable_iter != extra_drawables.end(); drawable_iter++) {
    (*drawable_iter)->transform->rotate_90();
  }
}


// Returns the audio clip that should be played when this object moves
std::optional<AudioManager::AudioClip> CellItem::get_move_clip() {
  return std::nullopt;
}


// If this CellItem has an audio clip for when it moves, push it onto the clips_to_play vector
void CellItem::push_move_clip() {
  std::optional<AudioManager::AudioClip> clip_opt = get_move_clip();
  if (clip_opt != std::nullopt) {
    AudioManager::clips_to_play.push(*clip_opt);
  }
}



/* ----- Background Tiles ----- */

// Doesn't apply to BgTile (for now)
bool BgTile::try_to_move_by(const glm::ivec2& displ) {
  // BgTiles can't move
  throw std::runtime_error("try_to_move_by() called on a BgTile. BgTiles can't move.");
  return false;
}


// Returns true iff the given foreground object is allowed to be moved/pushed onto this tile.
bool BgTile::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  // By default, any object can move onto this
  return true;
}


// Does whatever should happen when the given foreground object is moved/pushed onto this tile.
void BgTile::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ){
  // By default, do nothing
}


// Returns true iff a sky object can safely be moved/pushed into this cell
bool BgTile::can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ) {
  return true;
}


// Does whatever should happen when the given sky object is moved/pushed onto this tile.
void BgTile::when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ) {
  // By default, do nothing
}


// Returns true iff the input is handled somehow.
bool BgTile::on_input(const Input& input, Output* output){
  // By default, don't handle any input
  return false;
}



/* ----- Foreground Objects ----- */

// If this obj can be moved the distance and direction indicated by displ, then do so and return true.
// Otws return false.
bool FgObj::try_to_move_by(const glm::ivec2& displ) {
  glm::ivec2 target_pos = this->cell->pos + displ;
  if (!current_grid->is_valid_pos(target_pos)) return false;
  Cell* target_cell = current_grid->cell_at(target_pos);
  if (!target_cell->can_fg_obj_move_into(*this, displ)) {
  	return false;
  }
  target_cell->when_fg_obj_moved_into(*this, displ);
  if (target_cell->fgObj != nullptr) {
	  throw std::runtime_error("else here: Trying to move an FgObj into a cell that seems to still have one");
  }
  target_cell->set_fg_obj(this);
  return true;
}


// Returns true iff the given foreground object is allowed to be moved/pushed into this object.
// Default behavior is that this can be pushed according to displ.
bool FgObj::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  glm::ivec2 norm_displ = Grid::normalize_displ(displ);
  glm::ivec2 target_pos = this->cell->pos + norm_displ;
  // First check if this will end up outside the current grid.
  if (!current_grid->is_valid_pos(target_pos)) {
    return false;
  }
  // Otherwise, check if this can be pushed according to displ.
  return current_grid->cell_at(target_pos)->can_fg_obj_move_into(*this, norm_displ);
}


// Does whatever should happen when the given foreground object is moved/pushed into this object.
void FgObj::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  glm::ivec2 norm_displ = Grid::normalize_displ(displ);
  // TODO - is this... cool?
  if (!try_to_move_by(norm_displ) && !current_grid->rolling) {
  //if (!try_to_move_by(norm_displ)) {
    throw std::runtime_error("when_fg_obj_moved_into() somehow called for an object position & displacement that COULDN'T move.");
  }
  push_move_clip();
}


// Returns true iff a sky object can safely be moved/pushed into this cell.
bool FgObj::can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ) {
  return true;
}


// Does whatever should happen when the given sky object is moved/pushed into this obj.
void FgObj::when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ) {
  // By default, do nothing
}


// Returns true iff the input is handled somehow.
bool FgObj::on_input(const Input& input, Output* output){
  return false;
}



/* ----- Sky Objects ----- */

// If this obj can be moved the distance and direction indicated by displ, then do so and return true.
// Otws return false.
bool SkyObj::try_to_move_by(const glm::ivec2& displ) {
  glm::ivec2 target_pos = this->cell->pos + displ;
  if (!current_grid->is_valid_pos(target_pos)) return false;
  Cell* target_cell = current_grid->cell_at(target_pos);
  if (!target_cell->can_sky_obj_move_into(*this, displ)) return false;
  target_cell->when_sky_obj_moved_into(*this, displ);
  if (target_cell->skyObj != nullptr) {
    throw std::runtime_error("Trying to move a SkyObj into a cell that seems to still have one");
  }
  target_cell->set_sky_obj(this);
  return true;
}


// Returns true iff the given foreground object is allowed to be moved/pushed onto this tile.
bool SkyObj::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  return true;
}


// Does whatever should happen when the given foreground object is moved/pushed into this object.
void SkyObj::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  // By default, do nothing
}


// Returns true iff a sky object can safely be moved/pushed into this cell
bool SkyObj::can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ) {
  glm::ivec2 norm_displ = Grid::normalize_displ(displ);
  glm::ivec2 target_pos = this->cell->pos + norm_displ;
  // First check if this will end up outside the current grid.
  if (!current_grid->is_valid_pos(target_pos)) {
    return false;
  }
  // Otherwise, check if this can be moved according to displ.
  return current_grid->cell_at(target_pos)->can_sky_obj_move_into(*this, displ);
}


// Does whatever should happen when the given sky object is moved/pushed onto this tile.
void SkyObj::when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ) {
  glm::ivec2 norm_displ = Grid::normalize_displ(displ);
  if (!try_to_move_by(norm_displ)) {
    throw std::runtime_error("when_sky_obj_moved_into() somehow called for an object position & displacement that COULDN'T move.");
  }
  push_move_clip();
}


// Returns true iff the input is handled somehow.
bool SkyObj::on_input(const Input& input, Output* output) {
  return false;
}

#include "Grid.hpp"
#include <stdexcept>
#include <iostream>


// Variable declarations
Grid* current_grid = nullptr;



/* ----- Grid ----- */

// Grid constructor
Grid::Grid(size_t _width, size_t _height) : width(_width), height(_height) {
  cells = std::vector<std::vector<Cell>>(width, std::vector<Cell>(height, Cell(glm::ivec2(0))));
  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      cells[x][y].pos = glm::ivec2(x, y);
    }
  }
}


// Grid destructor
Grid::~Grid() {
}


// Returns true iff x and y are within the bounds of the grid
bool Grid::is_valid_pos(glm::ivec2 _pos) {
  return (_pos.x >= 0 && (size_t)_pos.x < this->width &&
          _pos.y >= 0 && (size_t)_pos.y < this->height);
}


// Calls the on_input function on every cell in the grid.
// Returns true iff something handled the input.
// For now, it only allows 1 CellItem to handle any input.
bool Grid::on_input(const Input& input) {
  for (size_t x = 0; x < width; x++) {
    for (size_t y = 0; y < height; y++) {
      if (cells[x][y].on_input(input)) return true;
    }
  }

  return false;
}



/* ----- Cell ----- */

// Cell constructor
Cell::Cell(glm::ivec2 _pos) : pos(_pos) {
};


// Cell destructor
Cell::~Cell() {
  if (bgTile) delete bgTile;
  if (fgObj) delete fgObj;
  if (skyObj) delete skyObj;
  bgTile = nullptr;
  fgObj = nullptr;
  skyObj = nullptr;
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
//  if (this->bgTile != nullptr && _bgTile != nullptr) throw std::runtime_error("Setting the bgTile of a cell that already has one.");
  this->bgTile = _bgTile;
  if (_bgTile == nullptr) return;

  if (_bgTile->cell != nullptr) {
    _bgTile->cell->bgTile = nullptr;
  }
  this->bgTile->cell = this;
  // TODO - smoothly move/animate this object?
  if (!this->bgTile->transform) throw std::runtime_error("No transform on a BgTile that's trying to move");
  this->bgTile->transform->position = glm::vec3(this->pos.x, this->pos.y, 0);
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
  // TODO - smoothly move/animate this object?
  if (!this->fgObj->transform) throw std::runtime_error("No transform on a FgObj that's trying to move");
  this->fgObj->transform->position = glm::vec3(this->pos.x, this->pos.y, 0);
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
  // TODO - smoothly move/animate this object?
  if (!this->skyObj->transform) throw std::runtime_error("No transform on a SkyObj that's trying to move");
  this->skyObj->transform->position = glm::vec3(this->pos.x, this->pos.y, 0);
}


// When there is some input, do something?
bool Cell::on_input(const Input& input) {
  return (bgTile != nullptr && bgTile->on_input(input)) |
         (fgObj != nullptr && fgObj->on_input(input)) |
         (skyObj != nullptr && skyObj->on_input(input));
}


// Returns true iff a foreground object can safely be moved/pushed into this cell
bool Cell::can_fg_obj_move_into(const FgObj& objBeingMoved, const glm::ivec2& displ) {
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



/* ----- Cell Items ----- */
// Nothing to implement. This is an abstract struct with pure virtual functions.



/* ----- Background Tiles ----- */

// Returns true iff the given foreground object is allowed to be moved/pushed onto this tile.
bool BgTile::can_fg_obj_move_into(const FgObj& objBeingMoved, const glm::ivec2& displ) {
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
bool BgTile::on_input(const Input& input){
  // By default, don't handle any input
  return false;
}



/* ----- Foreground Objects ----- */

// Returns true iff the given foreground object is allowed to be moved/pushed into this object.
// Default behavior is that this can be pushed according to displ.
bool FgObj::can_fg_obj_move_into(const FgObj& objBeingMoved, const glm::ivec2& displ) {
  glm::ivec2 target_pos = this->cell->pos + displ;
  // First check if this will end up outside the current grid.
  if (!current_grid->is_valid_pos(target_pos)) {
    return false;
  }
  // Otherwise, check if this can be pushed according to displ.
  return current_grid->cells.at(target_pos.x).at(target_pos.y).can_fg_obj_move_into(*this, displ);
}


// Does whatever should happen when the given foreground object is moved/pushed into this object.
void FgObj::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  glm::ivec2 target_pos = this->cell->pos + displ;
  // First double check if this will end up outside the current grid.
  if (!current_grid->is_valid_pos(target_pos)) {
    throw std::runtime_error("when_fg_moved_into() somehow called for an object position & displacement that would put it outside the current grid");
  }
  // Now move to the target cell
  Cell *target_cell = &current_grid->cells.at(target_pos.x).at(target_pos.y);
  target_cell->when_fg_obj_moved_into(*this, displ);
  if (target_cell->fgObj != nullptr) {
    throw std::runtime_error("Trying to move an FgObj into a cell that seems to still have one");
  }
  target_cell->set_fg_obj(this);
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
bool FgObj::on_input(const Input& input){
  return false;
}



/* ----- Sky Objects ----- */

// Returns true iff the given foreground object is allowed to be moved/pushed onto this tile.
bool SkyObj::can_fg_obj_move_into(const FgObj& objBeingMoved, const glm::ivec2& displ) {
  return true;
}


// Does whatever should happen when the given foreground object is moved/pushed into this object.
void SkyObj::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  // By default, do nothing
}


// Returns true iff a sky object can safely be moved/pushed into this cell
bool SkyObj::can_sky_obj_move_into(const SkyObj& objBeingMoved, const glm::ivec2& displ) {
  glm::ivec2 target_pos = this->cell->pos + displ;
  // First check if this will end up outside the current grid.
  if (!current_grid->is_valid_pos(target_pos)) {
    return false;
  }
  // Otherwise, check if this can be moved according to displ.
  return current_grid->cells.at(target_pos.x).at(target_pos.y).can_sky_obj_move_into(*this, displ);
}


// Does whatever should happen when the given sky object is moved/pushed onto this tile.
void SkyObj::when_sky_obj_moved_into(SkyObj& objBeingMoved, const glm::ivec2& displ) {
  glm::ivec2 target_pos = this->cell->pos + displ;
  // First double check if this will end up outside the current grid.
  if (!current_grid->is_valid_pos(target_pos)) {
    throw std::runtime_error("when_sky_moved_into() somehow called for an object position & displacement that would put it outside the current grid");
  }
  // Now move to the target cell
  Cell* target_cell = &current_grid->cells.at(target_pos.x).at(target_pos.y);
  target_cell->when_sky_obj_moved_into(*this, displ);
  if (target_cell->skyObj != nullptr) {
    throw std::runtime_error("Trying to move a SkyObj into a cell that seems to still have one");
  }
  target_cell->set_sky_obj(this);
}


// Returns true iff the input is handled somehow.
bool SkyObj::on_input(const Input& input) {
  return false;
}

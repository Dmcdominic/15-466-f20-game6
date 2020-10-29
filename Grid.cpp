#include "Grid.hpp"
#include <stdexcept>


/* ----- Grid ----- */

// Grid constructor
Grid::Grid(int width, int height) {
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


/* ----- Cell ----- */

// Cell constructor
Cell::Cell(glm::ivec2 _pos) : pos(_pos) {
};

// Cell destructor
Cell::~Cell() {
  if (bgTile) delete bgTile;
  if (fgObj) delete fgObj;
  bgTile = nullptr;
  fgObj = nullptr;
}

// When there is some input, do something?
bool Cell::on_input() {
  return (bgTile != nullptr && bgTile->on_input()) | (fgObj != nullptr && fgObj->on_input());
}

// Returns true iff a foreground object can safely be moved/pushed into this cell
bool Cell::can_fg_obj_move_into(const FgObj& objBeingMoved, const glm::ivec2& displ) {
  return (bgTile == nullptr || bgTile->can_fg_obj_move_onto(objBeingMoved, displ)) &&
         (fgObj == nullptr || fgObj->can_fg_obj_move_into(objBeingMoved, displ));
}

// Does whatever should happen when the given foreground object is moved/pushed into this cell.
// displ is a vector representing the displacement of the object being moved.
void Cell::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  if (bgTile != nullptr) {
    bgTile->when_fg_obj_moved_onto(objBeingMoved, displ);
  }
  if (fgObj != nullptr) {
    fgObj->when_fg_moved_into(objBeingMoved, displ);
  }
}


/* ----- Background Tiles ----- */

// Returns true iff the given foreground object is allowed to be moved/pushed onto this tile.
bool BgTile::can_fg_obj_move_onto(const FgObj& objBeingMoved, const glm::ivec2& displ) {
  // By default, any object can move onto this
  return true;
}

// Does whatever should happen when the given foreground object is moved/pushed onto this tile.
void BgTile::when_fg_obj_moved_onto(FgObj& objBeingMoved, const glm::ivec2& displ){
  // By default, do nothing
}

// Returns true iff the input is handled somehow.
bool BgTile::on_input(){
  // By default, don't handle any input
  return false;
}


/* ----- Foreground Tiles ----- */

// Returns true iff the given foreground object is allowed to be moved/pushed into this object.
// Default behavior is that this can be pushed according to displ
bool FgObj::can_fg_obj_move_into(const FgObj& objBeingMoved, const glm::ivec2& displ) {
  glm::ivec2 target_pos = this->cell->pos + displ;
  // First check if this will end up outside the current grid.
  if (target_pos.x >= current_grid->cells.size() || target_pos.x < 0 ||
      target_pos.y >= current_grid->cells[0].size() || target_pos.y < 0) {
    return false;
  }
  // Otherwise, check if this can be pushed according to displ.
  return current_grid->cells.at(target_pos.x).at(target_pos.y).can_fg_obj_move_into(*this, displ);
}

// Does whatever should happen when the given foreground object is moved/pushed into this object.
void FgObj::when_fg_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  glm::ivec2 target_pos = this->cell->pos + displ;
  // First double check if this will end up outside the current grid.
  if (target_pos.x >= current_grid->cells.size() || target_pos.x < 0 ||
    target_pos.y >= current_grid->cells[0].size() || target_pos.y < 0) {
    throw std::runtime_error("when_fg_moved_into() somehow called for an object position & displacement that would put it outside the current grid");
  }
  // Now move to the target cell
  Cell *target_cell = &current_grid->cells.at(target_pos.x).at(target_pos.y);
  target_cell->when_fg_obj_moved_into(*this, displ);
  if (target_cell->fgObj != nullptr) {
    throw std::runtime_error("Trying to move an FgObj into a cell that seems to still have one");
  }
  target_cell->fgObj = this;
  this->cell->fgObj = nullptr;
  this->cell = target_cell;
}

bool FgObj::on_input(){
  return false;
}

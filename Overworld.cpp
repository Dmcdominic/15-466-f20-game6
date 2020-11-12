#include "Overworld.hpp"
#include "Player.hpp"


// ----- OVERWORLD PATH -----

// When a player moves into a path tile, it should find the next path tile (or node) and send the player along.
void OverworldPath::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
  BgTile::when_fg_obj_moved_into(objBeingMoved, displ);
  if (dynamic_cast<Player*>(&objBeingMoved) != nullptr) {
    // TODO - find the next path tile or node
  }
}



// ----- OVERWORLD NODE -----

// No overrides
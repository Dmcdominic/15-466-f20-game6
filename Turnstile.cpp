#include "Turnstile.hpp"


// Called right after on_input for each cell
void Turnstile::on_post_tick(){
  rotate_90();
  if (cell->fgObj != nullptr) {
    cell->fgObj->rotate_90();
  }
}
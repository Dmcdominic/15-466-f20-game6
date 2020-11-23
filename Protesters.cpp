#include "Protesters.hpp"
#include "Barrel.hpp"
#include <iostream>


bool Protesters::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ){
    const FgObj* y = &objBeingMoved;
    const Barrel* x = dynamic_cast<const Barrel*>(y);
    if(x != nullptr) return true;
    return false;
}

void Protesters::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
    if(dynamic_cast<Barrel*>(&objBeingMoved) != nullptr) {
        this->drawables->remove(*(this->drawable));
        this->cell->fgObj= nullptr;
        delete this->cell->bgTile->drawable->transform;
        *(this->cell->bgTile->drawable) = this->rolled_over;
        this->cell->bgTile->drawable->transform->position = glm::vec3(this->cell->pos.x, this->cell->pos.y, 0);

        delete this;
    }
}
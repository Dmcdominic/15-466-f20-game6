#include "Tree.hpp"
#include "Barrel.hpp"
#include <iostream>


Scene::Drawable* Tree::prefab;

bool Tree::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ){
    const FgObj* y = &objBeingMoved;
    const Barrel* x = dynamic_cast<const Barrel*>(y);
    if(x != nullptr) return true;
    return false;
}

void Tree::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
    if(dynamic_cast<Barrel*>(&objBeingMoved) != nullptr) {
        this->drawables->remove(*(this->drawable));
        this->cell->fgObj= nullptr;
        *(this->cell->bgTile->drawable) = this->rotten;
        delete this;
    }
}
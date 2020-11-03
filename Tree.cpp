#include "Tree.hpp"
#include <iostream>


Scene::Drawable* Tree::prefab;

void Tree::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
    this->rotten.transform->position = this->current->transform->position;
    *(this->current) = this->rotten;
}
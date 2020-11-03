#include "Protesters.hpp"
#include <iostream>


Scene::Drawable* Protesters::prefab;

void Protesters::when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
    this->rolled_over.transform->position = this->current->transform->position;
    *(this->current) = this->rolled_over;
}
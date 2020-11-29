//
// Created by Jianrong Yu on 2020-11-16.
//

#include "Railing.hpp"

Railing::Railing(Scene *scene, bool left, bool right, bool upper, bool lower) {
    if(!left && right && !upper && lower){
        scene->drawables.push_back(model_loader->create_model("Railing_Bent"));
    }else if(!left && right && upper && !lower){
        scene->drawables.push_back(model_loader->create_model("Railing_Bent"));
    }else if(left && !right && upper && !lower){
        scene->drawables.push_back(model_loader->create_model("Railing_Bent"));
    }else if(left && !right && !upper && lower){
        scene->drawables.push_back(model_loader->create_model("Railing_Bent"));
    } else {
        scene->drawables.push_back(model_loader->create_model("Railing_Straight"));
    }
    this->drawable = &(scene->drawables.back());
}

bool Railing::can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) {
    return false;
}
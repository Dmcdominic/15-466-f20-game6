#include "Grid.hpp"
#include "Player.hpp"
#include "CloudCover.hpp"
#include <iostream>

CloudCover::CloudCover(Scene *_scene) : scene(_scene) {
    scene->drawables.push_back(model_loader->create_model("CloudCover"));
    left_cloud = &(scene->drawables.back());
    left_cloud->transform->position = left_position_uncovered;; 
   // left_cloud->transform->parent = current_grid->player->drawable->transform; 

    scene->drawables.push_back(model_loader->create_model("CloudCover"));
    right_cloud = &(scene->drawables.back());
    right_cloud->transform->position = right_position_uncovered; 
    //right_cloud->transform->parent = current_grid->player->drawable->transform; 
}

void CloudCover::cover(){
    covering = true; 
    moving = true; 
}

void CloudCover::uncover(){
    covering = false; 
    moving = true; 
}


void CloudCover::update(float elapsed){
    if(!moving) return; 
    if(holding) {
        holdElapsed += elapsed; 
        if(holdElapsed >= holdTime) {
            holdElapsed = 0.0f; 
            moving = false; 
            holding = false; 
        }
    }

    //move the clouds closer to the center
    else if(covering) {
        left_cloud->transform->position.x += movementSpeed * elapsed; 
        right_cloud->transform->position.x -= movementSpeed * elapsed; 
        //stop moving when the left cloud reaches its destination 
        if(left_cloud->transform->position.x >= left_position_covered.x) {
            holding = true; 
            // left_position_current = left_position_covered; 
            // right_position_current = right_position_covered; 

        }
    }

    //move the clouds away from the center 
    else {
        left_cloud->transform->position.x -= movementSpeed * elapsed; 
        right_cloud->transform->position.x += movementSpeed * elapsed; 
        
        //stop moving when the left cloud reaches its destination 
        if(left_cloud->transform->position.x <= left_position_uncovered.x) {
            moving = false; 
            // left_position_current = left_position_uncovered; 
            // right_position_current = right_position_uncovered;
        }
    }


}

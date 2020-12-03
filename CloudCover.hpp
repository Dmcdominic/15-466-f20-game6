#pragma once

#include "Scene.hpp"
#include "ModelLoader.hpp"


struct CloudCover {
    //Constructors 
    CloudCover(Scene *_scene); 

    //Fields
    Scene::Drawable *left_cloud = nullptr;
    Scene::Drawable *right_cloud = nullptr;
    Scene *scene; 

    float movementSpeed = 10.0f; 
    //the amount of time to wait with clouds covering the screen 
    float holdTime = 1.0f; 
    float holdElapsed = 0.0f; 
    bool covering = false; 
    bool moving = false; 
    bool holding = false; 
    glm::vec3 left_position_covered = glm::vec3(-3.1f, 0.0f, 1.5f); 
    glm::vec3 left_position_uncovered = glm::vec3(-10.0f, 0.0f, 1.5f); 
    glm::vec3 right_position_covered = glm::vec3(3.1f, 0.0f, 1.0f); 
    glm::vec3 right_position_uncovered = glm::vec3(10.0f, 0.0f, 1.0f); 

    //Methods
    void cover(); 
    void uncover(); 
    void update(float elapsed); 


};
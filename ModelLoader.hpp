#pragma once
#include "Scene.hpp"
#include <glm/glm.hpp>
#include <map>

struct ModelLoader{
    ModelLoader(); 
    unsigned int num_models = 5; 

    //returns a copy of the object with id model_id
    Scene::Drawable create_model(std::string model_name); 

    struct ModelTemplate{
        int id; 
        GLuint vao; 
        GLenum type; 
        GLuint start; 
        GLuint count; 
    }; 

    std::map <std::string, ModelTemplate> templates; 

	Scene scene;

}; 
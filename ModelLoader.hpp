#pragma once
#include "Scene.hpp"
#include <glm/glm.hpp>

struct ModelLoader{
    ModelLoader(); 
    unsigned int num_models = 5; 

    //returns a copy of the object with id model_id
    Scene::Drawable create_model(int model_id); 

    struct ModelTemplate{
        int id; 
        GLuint vao; 
        GLenum type; 
        GLuint start; 
        GLuint count; 
    }; 

    std::vector <ModelTemplate> templates; 

	Scene scene;

}; 
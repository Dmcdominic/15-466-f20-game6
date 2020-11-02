struct ModelLoader{
    ModelLoader(); 

    Scene::Drawable create_model(int model_id); 
    struct ModelTemplate{
        int id; 
        GLuint vao; 
        GLenum type; 
        GLuint start; 
        GLuint count; 
    }; 

    std::vector <ModelTemplate> templates; 

}; 
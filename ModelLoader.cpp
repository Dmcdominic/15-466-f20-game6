#include "ModelLoader.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "data_path.hpp"
#include "LitColorTextureProgram.hpp"
#include "LitToxicColorTextureProgram.hpp"
#include "LitPlantColorTextureProgram.hpp"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

ModelLoader* model_loader = nullptr;

GLuint toxic_meshes_for_lit_color_texture_program = 0;
GLuint toxic_meshes_for_lit_toxic_color_texture_program = 0;
GLuint toxic_meshes_for_lit_plant_color_texture_program = 0;

Load< MeshBuffer > meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("toxic-prefabs.pnct"));
	toxic_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	toxic_meshes_for_lit_toxic_color_texture_program = ret->make_vao_for_program(lit_toxic_color_texture_program->program);
	toxic_meshes_for_lit_plant_color_texture_program = ret->make_vao_for_program(lit_plant_color_texture_program->program);
	return ret;
});

Load< Scene > model_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("toxic-prefabs.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = meshes->lookup(mesh_name);

		scene.drawables.emplace_back(transform);
		Scene::Drawable &drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;

		drawable.pipeline.vao = toxic_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;
	});
});

ModelLoader::ModelLoader() : scene(*model_scene) {


    //TODO: save in ID order 
 	for (auto &drawable : scene.drawables) {
        ModelTemplate t; 
        t.type = drawable.pipeline.type; 
        t.start = drawable.pipeline.start; 
        t.count = drawable.pipeline.count; 
        // std::cout << "loaded " << drawable.transform->name << "\n"; 
        templates.insert(std::make_pair(drawable.transform->name, t)); 
    }

    //set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this

    //std::cout << "\nloaded " << templates.size() << " models\n"; 
}
    


Scene::Drawable ModelLoader::create_model(std::string model_name){
    std::map <std::string, ModelTemplate>::iterator it = templates.find(model_name); 
    if(it == templates.end()) {
        std::cout << "\n failed to find model " << model_name << ". Using default cube"; 
        it = templates.find("Cube"); 
    }
    ModelTemplate t = it->second;
    Scene::Transform *transform = new Scene::Transform; 
    Scene::Drawable drawable = Scene::Drawable(transform); 
    if(model_name == "Grass" || model_name.rfind("Tree", 0) == 0|| model_name.rfind("River", 0) == 0) {
        drawable.pipeline = lit_plant_color_texture_program_pipeline;
        drawable.pipeline.vao = toxic_meshes_for_lit_plant_color_texture_program;
    }
    else {
        drawable.pipeline = lit_toxic_color_texture_program_pipeline;
        drawable.pipeline.vao = toxic_meshes_for_lit_toxic_color_texture_program;
        drawable.pipeline.set_uniforms = [](){
            glUniform1f(lit_toxic_color_texture_program->PURPLE_AMT_float, 0.0f);
            glUniform1f(lit_toxic_color_texture_program->BROWN_AMT_float, 0.0f);
            glUniform1i(lit_toxic_color_texture_program->LIGHT_TYPE_int, 1);
            glUniform3fv(lit_toxic_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
            glUniform3fv(lit_toxic_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
        };
    }
    drawable.pipeline.type = t.type; 
    drawable.pipeline.start = t.start; 
    drawable.pipeline.count = t.count; 
    return drawable; 
}

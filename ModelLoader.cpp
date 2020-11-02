#include "ModelLoader.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "data_path.hpp"
#include "LitColorTextureProgram.hpp"
#include <iostream>


GLuint toxic_meshes_for_lit_color_texture_program = 0;

Load< MeshBuffer > meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("toxic-prefabs.pnct"));
	toxic_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
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
    std::cout << "loaded " << templates.size() << " models"; 
}
    


Scene::Drawable ModelLoader::create_model(std::string model_name){
    std::map <std::string, ModelTemplate>::iterator it = templates.find(model_name); 
    assert(it != templates.end()); 
    ModelTemplate t = it->second;
    Scene::Transform *transform = new Scene::Transform; 
    Scene::Drawable drawable = Scene::Drawable(transform); 
    
	drawable.pipeline = lit_color_texture_program_pipeline;
    drawable.pipeline.vao = toxic_meshes_for_lit_color_texture_program;
    drawable.pipeline.type = t.type; 
    drawable.pipeline.start = t.start; 
    drawable.pipeline.count = t.count; 
    return drawable; 
}

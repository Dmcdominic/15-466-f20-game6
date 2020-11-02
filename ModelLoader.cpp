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

    templates.reserve(num_models);

    //TODO: save in ID order 
 	for (auto &drawable : scene.drawables) {
        ModelTemplate t; 
        t.type = drawable.pipeline.type; 
        t.start = drawable.pipeline.start; 
        t.count = drawable.pipeline.count; 
        templates.push_back(t); 
    }
    std::cout << "loaded " << templates.size() << " models"; 
}
    


Scene::Drawable ModelLoader::create_model(int model_id){
    assert(model_id < templates.size()); 
    Scene::Transform *transform = new Scene::Transform; 
    Scene::Drawable &drawable = Scene::Drawable(transform); 
    
	drawable.pipeline = lit_color_texture_program_pipeline;
    drawable.pipeline.vao = toxic_meshes_for_lit_color_texture_program;
    drawable.pipeline.type = templates[model_id].type; 
    drawable.pipeline.start = templates[model_id].start; 
    drawable.pipeline.count = templates[model_id].count; 
    return drawable; 
}

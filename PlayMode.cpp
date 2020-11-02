#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "Player.hpp"
#include "Barrel.hpp"
#include "FixedRock.hpp"
#include "PushableBall.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <random>
#include <time.h>


GLuint toxic_prefabs_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > toxic_prefabs_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("toxic-prefabs.pnct"));
	toxic_prefabs_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > toxic_prefabs_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("toxic-prefabs.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = toxic_prefabs_meshes->lookup(mesh_name);

		scene.drawables.emplace_back(transform);
		Scene::Drawable &drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;

		drawable.pipeline.vao = toxic_prefabs_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;

	});
});


PlayMode::PlayMode() : scene(*toxic_prefabs_scene) {
	// First, seed the random number generator
	std::srand((unsigned int)time(NULL));

	// --- PREFAB ASSIGNMENT ---
	// TODO
	// Get pointers to prefabs
	for (auto& drawable : scene.drawables) {
		if (drawable.transform->name == "Player") {
			Player::prefab = &drawable;
		} else if (drawable.transform->name == "Barrel") {
			Barrel::prefab = &drawable;
		} else if (drawable.transform->name == "FixedRock") {
			FixedRock::prefab = &drawable;
		} else if (drawable.transform->name == "PushableBall") {
			PushableBall::prefab = &drawable;
		}
	}
	// Check if any prefabs are null
	if (Player::prefab == nullptr) {
		throw std::runtime_error("prefab Player not found");
	} else if (Barrel::prefab == nullptr) {
		throw std::runtime_error("prefab Barrel not found");
	}

	//create a camera
	scene.transforms.emplace_back();
	scene.cameras.emplace_back(&scene.transforms.back());
	active_camera = &scene.cameras.back();
	active_camera->fovy = glm::radians(60.0f);
	active_camera->near = 0.01f;

	active_camera->transform->position = glm::vec3(4.5f, 4.5f, 12.0f);
	active_camera->transform->rotation = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//rotate camera facing direction (-z) to player facing direction (+y):
	//playerOLD.camera->transform->rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// --- GRID INITIALIZATION ---
	//Construct a starting grid
	current_grid = new Grid(10, 10);

	// Fill it in with *placeholder* objects
	current_grid->cells.at(0).at(0).set_fg_obj(new Player(Player::prefab->transform));
	current_grid->cells.at(4).at(2).set_fg_obj(new Barrel(Barrel::prefab->transform));
	current_grid->cells.at(5).at(5).set_fg_obj(new FixedRock(FixedRock::prefab->transform));
	current_grid->cells.at(8).at(8).set_fg_obj(new PushableBall(PushableBall::prefab->transform));
}

PlayMode::~PlayMode() {
}


bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		} else if (evt.key.keysym.sym == SDLK_a ||
		           evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			if (current_grid != nullptr) current_grid->on_input(Input(InputType::LEFT));
			return true;
		} else if (evt.key.keysym.sym == SDLK_d ||
		           evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			if (current_grid != nullptr) current_grid->on_input(Input(InputType::RIGHT));
			return true;
		} else if (evt.key.keysym.sym == SDLK_w ||
		           evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			if (current_grid != nullptr) current_grid->on_input(Input(InputType::UP));
			return true;
		} else if (evt.key.keysym.sym == SDLK_s ||
		           evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			if (current_grid != nullptr) current_grid->on_input(Input(InputType::DOWN));
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a ||
		    evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d ||
		           evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w ||
		           evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s ||
		           evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_q) {  // QUIT
			this->quit = true;
			return true;
		}
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			return true;
		}
	}
	/*
	else if (evt.type == SDL_MOUSEMOTION) {
		if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
			glm::vec2 motion = glm::vec2(
				evt.motion.xrel / float(window_size.y),
				-evt.motion.yrel / float(window_size.y)
			);
			//glm::vec3 up = walkmesh->to_world_smooth_normal(playerOLD.at);
			glm::vec3 up = glm::vec3(0, 0, 1.0f);
			active_camera->transform->rotation = glm::angleAxis(-motion.x * active_camera->fovy, up) * active_camera->transform->rotation;

			float pitch = glm::pitch(active_camera->transform->rotation);
			pitch += motion.y * active_camera->fovy;
			//camera looks down -z (basically at the player's feet) when pitch is at zero.
			pitch = std::min(pitch, 0.95f * 3.1415926f);
			pitch = std::max(pitch, 0.05f * 3.1415926f);
			active_camera->transform->rotation = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));

			return true;
		}
	}
	*/

	return false;
}


void PlayMode::update(float elapsed) {
	// TODO - update stuff here?

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}


void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//update camera aspect ratio for drawable:
	active_camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	scene.draw(*active_camera);

	{ //use DrawLines to overlay some text:
		glDisable(GL_DEPTH_TEST);
		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
			1.0f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		));

		constexpr float H = 0.09f;
		lines.draw_text("Mouse motion looks; WASD moves; escape ungrabs mouse",
			glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		float ofs = 2.0f / drawable_size.y;
		lines.draw_text("Mouse motion looks; WASD moves; escape ungrabs mouse",
			glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + + 0.1f * H + ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
	}
	GL_ERRORS();
}

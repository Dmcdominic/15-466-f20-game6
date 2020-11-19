#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "Sound.hpp"

#include "GridLoader.hpp"

#include "Player.hpp"
#include "Barrel.hpp"
#include "Turnstile.hpp"
#include "Pit.hpp"
#include "Tree.hpp"
#include "Rock.hpp"

#include "PngView.hpp"

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

// Audio loading
Load< Sound::Sample > error_sample(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("Audio/Error1.wav"));
});

PlayMode::PlayMode() : scene(*toxic_prefabs_scene) {
	// First, seed the random number generator
	std::srand((unsigned int)time(NULL));

	//create a camera
	scene.transforms.emplace_back();
	scene.cameras.emplace_back(&scene.transforms.back());
	active_camera = &scene.cameras.back();
	active_camera->fovy = glm::radians(60.0f);
	active_camera->near = 0.01f;

	// Init camera position & rotation
	active_camera->transform->position = glm::vec3(2.0f, -1.0f, camera_height);
	active_camera->transform->rotation = glm::quat(glm::vec3(0.3f, 0.0f, 0.0f));

	// --- MODEL & GRID INITIALIZATION ---
	loader = ModelLoader(); 
	//change this value to view a different level
	current_grid = GridLoader::load_level(0, loader, &scene);

	// Manually throwing an object into the first grid, for testing
	/*scene.drawables.push_back(loader.create_model("Barrel"));
	current_grid->cells.at(4).at(1).set_fg_obj(new Barrel(&(scene.drawables.back())));

	BgTile *bgTileToRemove = current_grid->cells.at(4).at(1).bgTile;
	scene.drawables.remove(*bgTileToRemove->drawable);
	current_grid->cells.at(4).at(1).bgTile = nullptr;
	delete bgTileToRemove->drawable->transform;

	scene.drawables.push_back(loader.create_model("Turnstile"));
	current_grid->cells.at(4).at(1).set_bg_tile(new Turnstile(&(scene.drawables.back())));*/

	/*scene.drawables.push_back(loader.create_model("Pit"));
	current_grid->cells.at(4).at(2).set_bg_tile(new Pit(&(scene.drawables.back())));*/
}

PlayMode::~PlayMode() {
}


bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {

		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			/*SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;*/
		} else if (evt.key.keysym.sym == SDLK_LEFT || evt.key.keysym.sym == SDLK_a) {
			left_player.downs += 1;
			left_player.pressed = true;
			input_q.push(Input(InputType::LEFT));
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT || evt.key.keysym.sym == SDLK_d) {
			right_player.downs += 1;
			right_player.pressed = true;
			input_q.push(Input(InputType::RIGHT));
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP || evt.key.keysym.sym == SDLK_w) {
			up_player.downs += 1;
			up_player.pressed = true;
			input_q.push(Input(InputType::UP));
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN || evt.key.keysym.sym == SDLK_s) {
			down_player.downs += 1;
			down_player.pressed = true;
			input_q.push(Input(InputType::DOWN));
			return true;
		} else if (evt.key.keysym.sym == SDLK_e || evt.key.keysym.sym == SDLK_SPACE) {
			input_q.push(Input(InputType::INTERACT));
			return true;
		} else if (evt.key.keysym.sym == SDLK_q) {  // QUIT
			this->quit = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_n) {
			current_level = (current_level + 1) % num_levels; 
			current_grid = GridLoader::load_level(current_level, loader, &scene);
			return true;
		} else if (evt.key.keysym.sym == SDLK_r) {
			current_grid = GridLoader::load_level(current_level, loader, &scene);
			return true;
		}
		std::cout << "\n" << current_grid->num_disposed << "/" << current_grid->goal; 
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT || evt.key.keysym.sym == SDLK_a) {
			left_player.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT || evt.key.keysym.sym == SDLK_d) {
			right_player.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP || evt.key.keysym.sym == SDLK_w) {
			up_player.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN || evt.key.keysym.sym == SDLK_s) {
			down_player.pressed = false;
			return true;
		}
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		/*if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			return true;
		}*/
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
	// If the current grid isn't set, early-out.
	if (current_grid == nullptr) return;

	// Process input
	while (!input_q.empty()) {
		Output output = Output();
		bool handled = current_grid->on_input(Input(input_q.front()), &output);
		input_q.pop();
		if (!handled) {
			Sound::play_3D(*error_sample, 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
		}

		// Check if the output indicates a new level to load, e.g. they interacted with an overworld node.
		if (output.level_to_load) {
			current_level = (*output.level_to_load) % num_levels;
			current_grid = GridLoader::load_level(current_level, loader, &scene);
			break;
		}

		// Check if we should advance levels.
		if (current_grid->num_disposed >= current_grid->goal) {
            std::vector< MenuMode::Item > items;
            if (current_level+1 == num_levels) {
                update_congrats_items(items);
            } else {
                update_pass_items(items);
            }
            game_menu->update_items(items);
            Mode::switch_to_menu();
            current_level = (current_level + 1) % num_levels;
            current_grid = GridLoader::load_level(current_level, loader, &scene);
			break;
		}
	}

	// --- Move camera to follow player ---
	glm::vec3 target_cam_pos = current_grid->player->drawable->transform->position + camera_offset_from_player;
	glm::vec3 cam_displacement = target_cam_pos - active_camera->transform->position;
	float dist = glm::length(cam_displacement);
	float local_max_speed = glm::min(camera_max_speed, dist);

	// Update the camera's velocity, and upper bound by local_max_speed
	camera_velo += cam_displacement * camera_accel * elapsed * dist;
	if (glm::length(camera_velo) > local_max_speed) {
		camera_velo = local_max_speed * glm::normalize(camera_velo);
	}

	// If we're going in the wrong direction, slow it down
	if (glm::dot(camera_velo, cam_displacement) < 0.2f * glm::length(camera_velo)) {
		camera_velo *= 0.9f;
	}

	// Update the camera's position
	if (dist <= glm::length(camera_velo) * elapsed * 1.5f) {
		active_camera->transform->position = target_cam_pos;
		camera_velo = glm::vec3(0.0f);
	} else {
		active_camera->transform->position += camera_velo;
	}

	//reset button press counters:
	left_player.downs = 0;
	right_player.downs = 0;
	up_player.downs = 0;
	down_player.downs = 0;
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

	glClearColor(0.4f, 0.6f, .85f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	scene.draw(*active_camera);

	//draw environment meter png
	meter_png.draw();

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
		lines.draw_text("arrow keys or WASD to move, R to reset current level",
			glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		float ofs = 2.0f / drawable_size.y;
		lines.draw_text("arrow keys or WASD to move, R to reset current level",
			glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + + 0.1f * H + ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		lines.draw_text("environment score: " + std::to_string(current_grid->environment_score),
			glm::vec3(-aspect + 0.1f * H, -0.5 + 0.1f * H, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
		    glm::u8vec4(0xff, 0xff, 0xff, 0xff));
        lines.draw_text("Min Number of Barrels to Dispose: " + std::to_string(current_grid->goal),
                        glm::vec3(-aspect + 0.1f * H, 0.8 + 0.1f * H, 0.0),
                        glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
                        glm::u8vec4(0xff, 0xff, 0xff, 0xff));
	}
	GL_ERRORS();
}

void PlayMode::update_congrats_items(std::vector< MenuMode::Item > &items){
    items.emplace_back("Congratulations!");
    items.emplace_back("Restart from Level 0");
    items.back().on_select = [this](MenuMode::Item const &) {
        Mode::switch_to_play();
        current_level = (current_level + 1) % num_levels;
        current_grid = GridLoader::load_level(current_level, loader, &scene);
    };
    items.back().on_select = [](MenuMode::Item const&) {
        Mode::switch_to_play();
    };
}

void PlayMode::update_pass_items(std::vector< MenuMode::Item > &items){
    if (current_grid->environment_score==100)
        items.emplace_back("You nailed it! Your environmental score for this level is 100");
    else
        items.emplace_back("Try harder! Your environmental score for this level is"+std::to_string(current_grid->environment_score));
    items.emplace_back("Go to Level "+ std::to_string(current_level+1));
    items.back().on_select = [](MenuMode::Item const &) {
        Mode::switch_to_play();
    };
    items.back().on_select = [](MenuMode::Item const&) {
        Mode::switch_to_play();
    };
}



#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"
#include "LitToxicColorTextureProgram.hpp"
#include "LitPlantColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "Sound.hpp"

#include "GridLoader.hpp"
#include "ModelLoader.hpp"

#include "Player.hpp"
#include "Barrel.hpp"
#include "Turnstile.hpp"
#include "Pit.hpp"
#include "Tree.hpp"
#include "Rock.hpp"
#include "Overworld.hpp"

#include "AudioManager.hpp"
#include "PngView.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <random>
#include <time.h>


// GLuint toxic_prefabs_meshes_for_lit_color_texture_program = 0;

// Load< MeshBuffer > toxic_prefabs_meshes(LoadTagDefault, []() -> MeshBuffer const * {
// 	MeshBuffer const *ret = new MeshBuffer(data_path("toxic-prefabs.pnct"));
// 	toxic_prefabs_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
// 	toxic_meshes_for_lit_toxic_color_texture_program = ret->make_vao_for_program(lit_toxic_color_texture_program->program);
// 	return ret;
// });


Load< Scene > toxic_prefabs_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("toxic-prefabs.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		// Mesh const &mesh = toxic_prefabs_meshes->lookup(mesh_name);

		// scene.drawables.emplace_back(transform);
		// Scene::Drawable &drawable = scene.drawables.back();

		// drawable.pipeline = lit_color_texture_program_pipeline;

		// drawable.pipeline.vao = toxic_prefabs_meshes_for_lit_color_texture_program;
		// drawable.pipeline.type = mesh.type;
		// drawable.pipeline.start = mesh.start;
		// drawable.pipeline.count = mesh.count;

	});
});


// Static variable initialization
uint8_t PlayMode::current_level = 0;
uint8_t PlayMode::completed_level = 0;


// Constructor
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

	// Start the background music
	AudioManager::ensure_background_is_playing();

	// --- MODEL & GRID INITIALIZATION ---
	model_loader = new ModelLoader;
	load_level(0);
}

PlayMode::~PlayMode() {
}


bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {

		if (evt.key.keysym.sym == SDLK_ESCAPE) { // TODO - PAUSE MENU
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
		} else if (evt.key.keysym.sym == SDLK_e ||
		           evt.key.keysym.sym == SDLK_SPACE ||
							 evt.key.keysym.sym == SDLK_RETURN) { // INTERACT
			input_q.push(Input(InputType::INTERACT));
			return true;
		} else if (evt.key.keysym.sym == SDLK_q) {  // QUIT
			this->quit = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_r || evt.key.keysym.sym == SDLK_x) { // RESET
			input_q.push(Input(InputType::RESET));
			return true;
		} else if (evt.key.keysym.sym == SDLK_z) { // UNDO
			input_q.push(Input(InputType::UNDO));
			return true;
		} else if (evt.key.keysym.sym == SDLK_n) { // SKIP LEVEL. TODO - REMOVE THIS
			input_q.push(Input(InputType::SKIP_LVL));
			return true;
		}
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

	return false;
}


void PlayMode::update(float elapsed) {
	// If the current grid isn't set, early-out.
	if (current_grid == nullptr) return;

	// Player animation
	current_grid->player->on_update();

	// Process input
	while (!input_q.empty()) {
		Output output = Output();
		Input input = input_q.front();
		input_q.pop();
		if (input.type == InputType::RESET) {
			if (!is_Overworld()) { // Push an undo copy (Overworld excluded)
				undo_grids.push(GridLoader::create_undo_copy(current_grid));
			}
			load_level(current_level);
		} else if (input.type == InputType::SKIP_LVL) {
			load_level(current_level + 1);
		} else if (input.type == InputType::UNDO) {
			undo_move();
    } else if (level_completion&&input.type == InputType::INTERACT) {
      level_completion = false;
			environment_score += current_grid->grid_environment_score;
      // load the Overworld
      load_level(0);
			break;
		} else {
			if (!is_Overworld()) { // Push an undo copy (Overworld excluded)
				undo_grids.push(GridLoader::create_undo_copy(current_grid));
			}
			bool input_handled = current_grid->on_input(input, &output);
			if (!input_handled && !is_Overworld()) {
				delete undo_grids.top();
				undo_grids.pop();
			}
		}

		// Check if the output indicates a new level to load, e.g. they interacted with an overworld node.
		if (output.level_to_load) {
			load_level(*output.level_to_load);
			break;
		}

		// Check if the user passed.
		if (current_grid->num_disposed >= current_grid->goal) {
			completed_level = std::max(completed_level, current_level);
			level_completion = true;
		} else {
			level_completion = false;
		}
	}

	// Play audio
	while (!AudioManager::clips_to_play.empty()) {
		Sound::play_3D(AudioManager::get_sample(AudioManager::clips_to_play.front()), 0.2f, glm::vec3(0.0f, 0.0f, 0.0f));
		AudioManager::clips_to_play.pop();
	}

	// While the player has a forced move, move them.
	while (current_grid->player->next_forced_move != std::nullopt) {
		glm::ivec2 displ = *current_grid->player->next_forced_move;
		current_grid->player->next_forced_move = std::nullopt;
		current_grid->player->try_to_move_by(displ);
	}

	// --- Move camera to follow player ---
	glm::vec3 target_cam_pos = current_grid->player->drawable->transform->position + camera_offset_from_player;
	glm::vec3 cam_displacement = target_cam_pos - active_camera->transform->position;
	float dist = glm::length(cam_displacement);
	float local_max_speed = glm::min(get_cam_max_speed(), dist);

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

	// Update environment score meter
	if (environment_score >= 87) {
		png_meter = png_meter100;
	} else if (environment_score >= 62) {
		png_meter = png_meter75;
	} else if (environment_score >= 37) {
		png_meter = png_meter50;
	} else if (environment_score >= 12) {
		png_meter = png_meter25;
	} else {
		png_meter = png_meter0;
	}
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

	glUseProgram(lit_plant_color_texture_program->program);
	glUniform1i(lit_plant_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_plant_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_plant_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUniform1f(lit_plant_color_texture_program->ENVIRONMENT_HEALTH_float, std::clamp(float(environment_score) / 100, 0.0f, 1.0f));
	glUseProgram(0);

	glUseProgram(lit_toxic_color_texture_program->program);
	glUniform1i(lit_toxic_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_toxic_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_toxic_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUniform1f(lit_toxic_color_texture_program->ENVIRONMENT_HEALTH_float, std::clamp(float(environment_score) / 100, 0.0f, 1.0f));

	glUseProgram(0);

	glClearColor(0.4f, 0.6f, .85f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	scene.draw(*active_camera);

	//draw environment meter png
	png_meter->draw();

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
//		lines.draw_text(std::to_string(environment_score),
//			glm::vec3(-aspect + 0.335 + 0.1f * H, -0.76 + 0.1f * H, 0.0),
//			glm::vec3(0.7 * H, 0.0f, 0.0f), glm::vec3(0.0f, 0.7 * H, 0.0f),
//		    glm::u8vec4(0xff, 0xff, 0xff, 0xff));
        lines.draw_text("remaining: " + std::to_string(current_grid->goal - current_grid->num_disposed),
                        glm::vec3(-aspect + 0.52 + 0.1f * H, 0.75 + 0.1f * H, 0.0),
                        glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
                        glm::u8vec4(0xff, 0xff, 0xff, 0xff));
		lines.draw_text("move",
		                glm::vec3(-aspect + 2.75 + 0.1f * H, -0.55 + 0.1f * H, 0.0),
		                glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
		                glm::u8vec4(0xff, 0xff, 0xff, 0xff));
		lines.draw_text("reset level",
		                glm::vec3(-aspect + 2.67 + 0.1f * H, -0.87 + 0.1f * H, 0.0),
		                glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
		                glm::u8vec4(0xff, 0xff, 0xff, 0xff));
        if (level_completion) lines.draw_text("Congratulations! Press ENTER/SPACE to go back to OverWorld",
                        glm::vec3(-aspect+0.8, 0.0, 0.0),
                        glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
                        glm::u8vec4(0xff, 0xff, 0xff, 0xff));
	}
	GL_ERRORS();
}


// Loads a level using the GridLoader
void PlayMode::load_level(uint8_t level_index) {
	bool resetting = (level_index == current_level);
	current_level = level_index % num_levels;
	
	current_grid = GridLoader::load_level(current_level, &scene);
	if (is_Overworld() && current_grid->highest_level_node != nullptr) {
		// Find the position of the node you should teleport the player to
		if (current_grid->highest_level_node->cell->fgObj != current_grid->player) {
			assert(current_grid->highest_level_node->cell->fgObj == nullptr);
			current_grid->highest_level_node->cell->set_fg_obj(current_grid->player);
		}
	}
	if (!resetting) {
		clear_undo_stack();
	}
	if (!resetting || is_Overworld()) {
		glm::vec3 offset = reset_cam_offset_from_player();
		active_camera->transform->position = current_grid->player->drawable->transform->position + offset;
		camera_velo = glm::vec3();
	}
}


// Reverts the grid to its state before the player's last input.
// Returns true iff a valid undo was executed.
bool PlayMode::undo_move() {
	if (undo_grids.empty()) {
		AudioManager::clips_to_play.push(AudioManager::AudioClip::ERROR);
		return false;
	}
	Grid* last_grid = undo_grids.top();
	undo_grids.pop();
	GridLoader::unload_current_grid(&scene);
	// Load the undo copy grid
	GridLoader::load_undo_copy(last_grid, &scene);
	// TODO - play a sound or something?
	return true;
}


// Cleans up the whole stack of undo grids
void PlayMode::clear_undo_stack() {
	while (!undo_grids.empty()) {
		delete undo_grids.top();
		undo_grids.pop();
	}
}

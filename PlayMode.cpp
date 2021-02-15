#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"
#include "LitToxicColorTextureProgram.hpp"
#include "LitPlantColorTextureProgram.hpp"
#include "Framebuffers.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "Sound.hpp"

#include "GridLoader.hpp"
#include "ModelLoader.hpp"
#include "level_sequence.hpp"

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
#include <fstream>


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
bool PlayMode::DEBUG = false;
uint8_t PlayMode::current_level = 0;
uint8_t PlayMode::completed_level = 0;
glm::ivec2 PlayMode::last_node_pos = glm::ivec2();


// Constructor
PlayMode::PlayMode(uint8_t _completed_level, int _environment_score) : scene(*toxic_prefabs_scene), environment_score(_environment_score) {
	// First, seed the random number generator

	completed_level = _completed_level;
	std::srand((unsigned int)time(NULL));

	//create a camera
	scene.transforms.emplace_back();
	scene.cameras.emplace_back(&scene.transforms.back());
	active_camera = &scene.cameras.back();
	active_camera->fovy = glm::radians(60.0f);
	active_camera->near = 0.01f;

	// Init camera position & rotation
	active_camera->transform->position = glm::vec3(2.0f, -1.0f, min_cam_height);
	active_camera->transform->rotation = glm::quat(glm::vec3(0.3f, 0.0f, 0.0f));

	// Start the background music
	AudioManager::ensure_background_is_playing();

	// --- MODEL & GRID INITIALIZATION ---
	model_loader = new ModelLoader;

	load_level(0);
	menu = new Menu(pngHelper);

	// Create cloud cover (for loading)
	cloud_scene = Scene();
	cloud_cover = new CloudCover(&cloud_scene);
	cloud_scene.transforms.emplace_back();
	cloud_scene.cameras.emplace_back(&cloud_scene.transforms.back());
	cloud_camera = &cloud_scene.cameras.back();
	cloud_camera->fovy = glm::radians(60.0f);
	cloud_camera->near = 0.01f;
	cloud_camera->transform->position = glm::vec3(0.0f, 0.0f, 5.0f);
	cloud_camera->transform->rotation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
}

PlayMode::~PlayMode() {
}


bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	if (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_q) { // QUIT - check before !loading_level check
		this->quit = true;
		return true;
	}

	if (!loading_level && evt.type == SDL_KEYDOWN) {

		if (evt.key.keysym.sym == SDLK_ESCAPE) {
			input_q.push(Input(InputType::ESCAPE));
			return true;
			/*SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;*/
		} else if (evt.key.keysym.sym == SDLK_LEFT || evt.key.keysym.sym == SDLK_a) {
			input_q.push(Input(InputType::LEFT));
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT || evt.key.keysym.sym == SDLK_d) {
			input_q.push(Input(InputType::RIGHT));
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP || evt.key.keysym.sym == SDLK_w) {
			input_q.push(Input(InputType::UP));
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN || evt.key.keysym.sym == SDLK_s) {
			input_q.push(Input(InputType::DOWN));
			return true;
		} else if (evt.key.keysym.sym == SDLK_e ||
		           evt.key.keysym.sym == SDLK_SPACE ||
		           evt.key.keysym.sym == SDLK_RETURN) { // INTERACT
			input_q.push(Input(InputType::INTERACT));
			return true;
		} else if (evt.key.keysym.sym == SDLK_r || evt.key.keysym.sym == SDLK_x) { // RESET
			input_q.push(Input(InputType::RESET));
			return true;
		} else if (evt.key.keysym.sym == SDLK_z) { // UNDO
			input_q.push(Input(InputType::UNDO));
			return true;
		} else if (evt.key.keysym.sym == SDLK_n) { // SKIP TO NEXT LEVEL
			if (PlayMode::DEBUG) {
				input_q.push(Input(InputType::SKIP_LVL));
				return true;
			}
		} else if (evt.key.keysym.sym == SDLK_p) { // BACK TO PREVIOUS LEVEL
			if (PlayMode::DEBUG) {
				input_q.push(Input(InputType::PREV_LVL));
				return true;
			}
		}
		else if (evt.key.keysym.sym == SDLK_o) { // JUMP TO OVERWORLD
			if (PlayMode::DEBUG) {
				input_q.push(Input(InputType::JUMP_TO_OW));
				return true;
			}
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT || evt.key.keysym.sym == SDLK_a) {
			//return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT || evt.key.keysym.sym == SDLK_d) {
			//return true;
		} else if (evt.key.keysym.sym == SDLK_UP || evt.key.keysym.sym == SDLK_w) {
			//return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN || evt.key.keysym.sym == SDLK_s) {
			//return true;
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

	// don't process input yet if clouds are moving
	if(loading_level) {
		cloud_cover->update(elapsed);

		//if the clouds are done moving
		if(!cloud_cover->moving && !cloud_cover->covering) {
			loading_level = false;
		}

			//if the clouds need to start moving outwards
		else if(!cloud_cover->moving && cloud_cover->covering) {
			cloud_cover->uncover();
			if (load_main_menu) {
				load_main_menu = false;
				menu->setSNode(menu->sNodes[(size_t)Menu::MENUS::MAIN_MENU]);
			} else if (load_credits) {
				load_credits = false;
				menu->setSNode(menu->sNodes[(size_t)Menu::MENUS::CREDITS]);
			} else if (level_to_load >= num_levels) {
				menu->setSNode(menu->sNodes[(size_t)Menu::MENUS::CREDITS]);
			} else {
				load_level(level_to_load);
			}
		}
		//camera_velo = glm::vec3(0.0f);
		update_camera(elapsed);
		return;
	}

	// Roll any barrels that should be rolled, and process post_tick() if they just finished
	bool barrels_still_rolling = Barrel::roll_rolling_barrels(current_grid);
	if (current_grid->post_tick_queued && !barrels_still_rolling) {
		current_grid->post_tick();
	}

	// Process input
	if (!current_grid->post_tick_queued && !input_q.empty()) {
		// Pop down to only the most recent input
		Input input = input_q.front();
		while (!input_q.empty()) {
			input = input_q.front();
			input_q.pop();
		}

		Output output = Output();

		// Menu case
		if (menu->current_sNode != nullptr) {
			if (input.type == InputType::DOWN) {
				menu->select_up_down(1);
				AudioManager::clips_to_play.push(AudioManager::AudioClip::TOGGLE);
			} else if (input.type == InputType::UP) {
				menu->select_up_down(-1);
				AudioManager::clips_to_play.push(AudioManager::AudioClip::TOGGLE);
			} else if (input.type == InputType::INTERACT) {
				Menu::Item &item = menu->current_sNode->items[menu->current_sNode->selected];
				item.on_select(item);
				AudioManager::clips_to_play.push(AudioManager::AudioClip::SELECT);
			} else if (input.type == InputType::ESCAPE) {
				if (menu->current_sNode == menu->sNodes[(size_t)Menu::MENUS::PAUSE_LVL] ||
						menu->current_sNode == menu->sNodes[(size_t)Menu::MENUS::PAUSE_OW]) {
					menu->disableMenu();
					AudioManager::clips_to_play.push(AudioManager::AudioClip::SELECT);
				}
			} else if (input.type == InputType::RESET) {
				reset_level();
			}

			// Now check if things should change
			if (menu->quit_asap) {
				menu->quit_asap = false;
				this->quit = true;
				return;
			} else if (menu->play) {
				menu->play = false;
				level_to_load = 0;
				loading_level = true;
				cloud_cover->cover();
			} else if (menu->restart_level) {
				menu->restart_level = false;
				reset_level();
			} else if (menu->new_game) {
				menu->new_game = false;
				completed_level = 0;
				environment_score = 100;
				level_to_load = 0;
				last_node_pos = glm::ivec2();
				loading_level = true;
				save_game();
				cloud_cover->cover();
			} else if (menu->return_to_OW) {
				menu->return_to_OW = false;
				level_to_load = 0;
				loading_level = true;
				cloud_cover->cover();
			} else if (menu->load_main_menu) {
				menu->load_main_menu = false;
				load_main_menu = true;
				loading_level = true;
				cloud_cover->cover();
			}
		}

		// Main case (in the overworld or a level, not in a menu)
		else {
			if (input.type == InputType::RESET) {
				reset_level();
			}
			else if (input.type == InputType::SKIP_LVL) {
				load_level(current_level + 1);
			}
			else if (input.type == InputType::PREV_LVL) {
				load_level(current_level - 1);
			}
			else if (input.type == InputType::JUMP_TO_OW) {
				load_level(0);
			}
			else if (input.type == InputType::UNDO) {
				undo_move();
			}
			else if (input.type == InputType::ESCAPE) {
				Menu::MENUS menuToLoad = is_Overworld() ? Menu::MENUS::PAUSE_OW : Menu::MENUS::PAUSE_LVL;
				menu->setSNode(menu->sNodes[(size_t)menuToLoad]);
				AudioManager::clips_to_play.push(AudioManager::AudioClip::TOGGLE);
			}
			else if (level_completion && input.type == InputType::INTERACT) {
				environment_score += current_grid->grid_environment_score;
				// load the Overworld (or credits if this was last level)
				level_to_load = 0;
				if (current_level == num_levels - 1) {
					load_credits = true;
				}
				loading_level = true;
				cloud_cover->cover();
			}
			else {
				if (!is_Overworld()) { // Push an undo copy (Overworld excluded)
					undo_grids.push(GridLoader::create_undo_copy(current_grid));
				}
				bool input_handled = current_grid->on_input(input, &output);
				if (!input_handled && !is_Overworld()) {
					delete undo_grids.top();
					undo_grids.pop();
				}
			}
		}

		// Check if the output indicates a new level to load, e.g. they interacted with an overworld node.
		if (output.last_node_pos != glm::ivec2()) {
			last_node_pos = output.last_node_pos;
		}
			
		if (output.level_to_load) {
			cloud_cover->cover();
			level_to_load = *output.level_to_load;
			loading_level = true;
		}

		// Check if level is complete
		check_level_completion();
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

	// Check if level is complete
	check_level_completion();

	// --- Move camera to follow player ---
	update_camera(elapsed);

	// Update environment score meter
	pngHelper->update_env_score(environment_score);
}


void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//https://github.com/15-466/15-466-f20-framebuffer/blob/master/PlayMode.cpp framebuffer code
	//make sure framebuffers are the same size as the window:
	framebuffers.realloc(drawable_size);

	pngHelper->update_png_pos(drawable_size);
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

	//---- draw scene to HDR framebuffer ----
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffers.hdr_fb);

	glClearColor(0.4f, 0.6f, .85f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	scene.draw(*active_camera);

	//draw png's
	pngHelper->draw(!is_Overworld(), // draw barrels in levels
	                (!level_completion && (completed_level != current_level)), // draw WASD if player can move
	                (level_completion && !loading_level && !is_Overworld()), // draw return after level completes
	                is_Overworld(), // draw select at overworld
	                (!is_Overworld() && !level_completion && (completed_level != current_level)), // reset during game
	                current_grid->num_disposed, current_grid->goal, current_level, // for drawing faded/filled barrels
	                menu // for drawing whatever menu is open
	);

	//draw cloud overlay
	cloud_scene.draw(*cloud_camera);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//copy scene to main window framebuffer:
	
	framebuffers.tone_map();

	//tried adding anti-aliasing, but it looks funny :( 
	//framebuffers.fxaa(drawable_size);

	 //use DrawLines to overlay debugging text
	if (PlayMode::DEBUG) {
		glDisable(GL_DEPTH_TEST);
		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
				1.0f / aspect, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
		));

		auto draw_text = [&](glm::vec2 const& at, std::string const& text, float H) {
			lines.draw_text(text,
				glm::vec3(at.x, at.y, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0x00, 0x00, 0x00, 0x00));
			float ofs = 2.0f / drawable_size.y;
			lines.draw_text(text,
				glm::vec3(at.x + ofs, at.y + ofs, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		};

		std::string lvl_display = std::to_string(current_level) + " - " + level_sequence[current_level].name;
		draw_text(glm::vec2(-aspect + 0.05f, -0.9f), lvl_display, 0.09f);
	}

	GL_ERRORS();
}

// Loads a level using the GridLoader
void PlayMode::load_level(uint8_t level_index) {
	if (menu != nullptr) {
		menu->disableMenu();
	}
	bool resetting = (level_index == current_level);
	current_level = level_index % num_levels;

	current_grid = GridLoader::load_level(current_level, &scene);
	if (is_Overworld() && last_node_pos != glm::ivec2()) {
		// Find the position of the node you should teleport the player to
		Cell* targetCell = current_grid->cell_at(last_node_pos);
		if (targetCell->fgObj != current_grid->player) {
			assert(targetCell->fgObj == nullptr);
			targetCell->set_fg_obj(current_grid->player);
		}
		last_node_pos = glm::ivec2();
	} else if (is_Overworld() && current_grid->highest_level_node != nullptr) {
		// Find the position of the node you should teleport the player to
		if (current_grid->highest_level_node->cell->fgObj != current_grid->player) {
			assert(current_grid->highest_level_node->cell->fgObj == nullptr);
			current_grid->highest_level_node->cell->set_fg_obj(current_grid->player);
		}
	}
	if (!resetting) {
		clear_undo_stack();
	}
	reset_cam_offset_from_player();
}


// Resets (restarts) the level
void PlayMode::reset_level() {
	if (!is_Overworld()) { // Push an undo copy (Overworld excluded)
		undo_grids.push(GridLoader::create_undo_copy(current_grid));
	}
	pngHelper->reset();
	// load_level(current_level);
	level_to_load = current_level;
	loading_level = true;
	cloud_cover->cover();
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
	AudioManager::clips_to_play.push(AudioManager::AudioClip::UNDO);
	return true;
}


// Cleans up the whole stack of undo grids
void PlayMode::clear_undo_stack() {
	while (!undo_grids.empty()) {
		delete undo_grids.top();
		undo_grids.pop();
	}
}


// Sets level_completion to true if current_grid goal is satisfied, false otherwise
void PlayMode::check_level_completion() {
	if (!is_Overworld() && current_grid->num_disposed >= current_grid->goal) {
		completed_level = std::max(completed_level, current_level);
		pngHelper->draw();
		if (!level_completion) {
			AudioManager::clips_to_play.push(AudioManager::AudioClip::VICTORY);
      save_game();
		}
		level_completion = true;

	} else {
		level_completion = false;
	}
}


// Saves the current level progress to file
void PlayMode::save_game() {
	std::fstream out;
	out.open(data_path("save0.enviro"), std::fstream::out);
	out << (int)completed_level << " " << environment_score << std::endl;
	out.close();
}


// Updates the camera
void PlayMode::update_camera(float elapsed) {
	if (menu->current_sNode != nullptr) return;

	// NEW CAMERA LOGIC
	// Move the smooth player target for camera
	glm::vec3 smooth_target_target = current_grid->player->drawable->transform->position;

	glm::vec3 target_displacement = smooth_target_target - smooth_player_target_for_cam;
	glm::vec3 target_velo = target_displacement * 4.5f * glm::min(0.9f, 1.7f * elapsed);
	smooth_player_target_for_cam += target_velo;

	// FOR TESTING
	//current_grid->player->smooth_target_0->transform->position = smooth_player_target_for_cam;
	//current_grid->player->smooth_target_1->transform->position = smooth_player_target_for_cam;

	// Move the camera itself
	glm::vec3 target_cam_pos = smooth_player_target_for_cam + camera_offset_from_player;
	glm::vec3 cam_displacement = target_cam_pos - active_camera->transform->position;

	glm::vec3 camera_velo = cam_displacement * glm::min(0.9f, 1.7f * elapsed);
	active_camera->transform->position += camera_velo;
}


// Resets the camera offset from the player
void PlayMode::reset_cam_offset_from_player() {
	//smooth_player_target_for_cam = current_grid->player->drawable->transform->position;
	float scalar = (std::max(current_grid->width, current_grid->height) - min_grid_width) / (float)(max_grid_width - min_grid_width);
	float z = min_cam_height + scalar * (max_cam_height - min_cam_height);
	if (is_Overworld()) z = cam_height_OW;
	camera_offset_from_player = glm::vec3(base_cam_offset_from_player.x, base_cam_offset_from_player.y, z);
	float rand_scale = -1.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f;

	glm::vec3 offset = camera_offset_from_player + randomized_offset_range * rand_scale;

	active_camera->transform->position = current_grid->player->drawable->transform->position + offset;
	smooth_player_target_for_cam = current_grid->player->drawable->transform->position;
}

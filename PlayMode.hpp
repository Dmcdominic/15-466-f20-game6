#include "Mode.hpp"

#include "Scene.hpp"
#include "GridLoader.hpp"
#include "Grid.hpp"
#include "ModelLoader.hpp"
#include "game_menu.hpp"
#include "level_sequence.hpp"

#include "PngView.hpp"

#include <glm/glm.hpp>

#include <queue>


struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

  void update_congrats_items(std::vector< MenuMode::Item > &items);
  void update_pass_items(std::vector< MenuMode::Item > &items);

	void load_level(uint8_t level_index);
	bool undo_move();
	void clear_undo_stack();

	void update_png_pos(float drawable_aspect);


	//----- game state -----

	static uint8_t current_level; 
	static uint8_t completed_level;
	static bool is_Overworld() {
		return current_level == 0;
	}

	ModelLoader loader; 

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left_player, right_player, down_player, up_player;

	std::queue<Input> input_q;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;

	// Active camera
	Scene::Camera *active_camera = nullptr;

	float camera_height = 8.0f;
	float camera_height_OW = 10.0f;
	glm::vec2 base_cam_offset_from_player = glm::vec2(-0.2f, -2.0f);
	glm::vec3 camera_offset_from_player = glm::vec3();
	glm::vec3 randomized_offset_range = glm::vec3(1.5f, 1.5f, 2.0f);

	float camera_max_speed = 0.05f;
	float camera_max_speed_OW = 0.09f;
	float camera_accel = 0.15f;
	glm::vec3 camera_velo = glm::vec3();

	float get_cam_max_speed() {
		return (is_Overworld()) ? camera_max_speed_OW : camera_max_speed;
	}
	glm::vec3 reset_cam_offset_from_player() {
		float z = (is_Overworld()) ? camera_height_OW : camera_height;
		camera_offset_from_player = glm::vec3(base_cam_offset_from_player.x, base_cam_offset_from_player.y, z);
		float rand_scale = -1.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f;
		return camera_offset_from_player + randomized_offset_range * rand_scale;
	}

	int environment_score = 100;
	bool level_completion = false;
	glm::uvec2 prev_drawable_size = glm::uvec2(0, 0);
				  /* lower left triangle */  /* upper right triangle */
	float meter_xs[6] = {-1.0f, -1.0f, -0.7f, -1.0f, -0.7f, -0.7f};
	float meter_ys[6] = {-0.5f, -1.0f, -1.0f, -0.5f, -1.0f, -0.5f};
	PngView *png_meter100 = new PngView("meter100.png", meter_xs, meter_ys);
	PngView *png_meter75 = new PngView("meter75.png", meter_xs, meter_ys);
	PngView *png_meter50 = new PngView("meter50.png", meter_xs, meter_ys);
	PngView *png_meter25 = new PngView("meter25.png", meter_xs, meter_ys);
	PngView *png_meter0 = new PngView("meter0.png", meter_xs, meter_ys);
	PngView *png_meter = png_meter0;
	float barrel_aspect = 1.0f;
	float barrel_w = 0.3;
	float barrel_xs[6] = {-1.0f, -1.0f, barrel_w - 1.0f, -1.0f, barrel_w - 1.0f, barrel_w - 1.0f};
	float barrel_ys[6] = {1.0f, 1 - barrel_w, 1 - barrel_w, 1.0f, 1 - barrel_w, 1.0f};
	PngView *png_barrel = new PngView("barrel.png", barrel_xs, barrel_ys);
};

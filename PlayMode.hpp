#include "Mode.hpp"

#include "Scene.hpp"
#include "WalkMesh.hpp"
#include "GridLoader.hpp"
#include "Grid.hpp"
#include "ModelLoader.hpp"


#include <glm/glm.hpp>

#include <queue>


struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	uint8_t current_level = 0; 
	//TODO: get this from assets, not hardcoded
	uint8_t num_levels = 8; 
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

	float camera_height = 7.5f;
	glm::vec3 camera_offset_from_player = glm::vec3(-0.2f, -2.0f, camera_height);

	float camera_max_speed = 0.05f;
	float camera_accel = 0.15f;
	glm::vec3 camera_velo = glm::vec3();
};

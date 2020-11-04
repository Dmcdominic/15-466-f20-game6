#include "Mode.hpp"

#include "Scene.hpp"
#include "WalkMesh.hpp"
#include "GridLoader.hpp"
#include "Grid.hpp"
#include "ModelLoader.hpp"


#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	uint8_t current_level = 0; 
	uint8_t num_levels = 2; 
	ModelLoader loader; 

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left_player, right_player, down_player, up_player, left_camera, right_camera, down_camera, up_camera;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;

	// Active camera
	Scene::Camera *active_camera = nullptr;
	float camera_move_amount = .1f; 
};

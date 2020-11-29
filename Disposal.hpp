#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"

struct Disposal : BgTile {
	// Constructor
	Disposal(Scene* _scene);

	// Fields
	Scene::Drawable *grass = nullptr;

	// Methods
	void load_models(Scene* scene) override;
	Disposal* clone_lightweight(Cell* new_cell) override;

	bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
	void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
};

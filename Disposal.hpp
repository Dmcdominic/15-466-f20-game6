#include "Grid.hpp"
#include "Scene.hpp"
#include "ModelLoader.hpp"

struct Disposal : BgTile {
		// Constructors (inherited)

		// Fields
		Scene::Drawable *grass;
        Scene* scene = nullptr;

		// Methods
  		Disposal(Scene *_scene); 

		bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
		void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
		void position_models();

};

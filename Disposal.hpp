#include "Grid.hpp"
#include "Scene.hpp"

struct Disposal : BgTile {
		// Constructors (inherited)

		// Fields
		static Scene::Drawable* prefab;
        std::list<Scene::Drawable>* drawables = nullptr;

		// Methods
		Disposal(Scene::Drawable* _drawable, std::list<Scene::Drawable>* _drawables): BgTile(_drawable), drawables(_drawables){};

		bool can_fg_obj_move_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
		void when_fg_obj_moved_into(FgObj& objBeingMoved, const glm::ivec2& displ) override;
};

#include "game_menu.hpp"
#include "Load.hpp"
#include "data_path.hpp"

#include "PlayMode.hpp"


std::shared_ptr< MenuMode > game_menu;

Load< void > load_game_menu(LoadTagDefault, []() {
	std::vector< MenuMode::Item > items;
	items.emplace_back("Start Game");
	items.back().on_select = [](MenuMode::Item const&) {
	    Mode::switch_to_play();
	};

	game_menu = std::make_shared< MenuMode >(items);
	game_menu->draw_items();
	game_menu->selected = 0;
	game_menu->view_min = glm::vec2(0.0f, 0.0f);
	game_menu->view_max = glm::vec2(320.0f, 200.0f);

	game_menu->layout_items(2.0f);
	game_menu->left_select_offset = glm::vec2(-5.0f - 3.0f, 0.0f);
	game_menu->right_select_offset = glm::vec2(0.0f + 3.0f, 0.0f);
	game_menu->select_bounce_amount = 5.0f;
});

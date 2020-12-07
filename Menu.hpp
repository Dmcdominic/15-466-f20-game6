#pragma once

#include <string>
#include <functional>

#include "PngView.hpp"

// Forward declaration
struct PngHelper;


// Based on my MenuMode story graph structure in Primordials (game4)
//   https://github.com/Dmcdominic/Primordials/blob/master/MenuMode.hpp
struct Menu {
	// Constructor
	Menu(PngHelper* pngHelper);

	bool quit_asap = false;
	bool restart_level = false;
	bool return_to_OW = false;
	bool new_game = false;
	bool play = false;
	bool load_main_menu = false;

	// The different menu screens (SNodes)
  enum class MENUS { MAIN_MENU, CREDITS, PAUSE, END_SCREEN, MAX };

	// Types of items
	enum class ITEM_TYPE {
		IMAGE, OPTION
	};

	//Each menu item is an "Item":
	struct Item {
		Item(
			PngView* pngView_,
			PngView* pngView_selected_,
			std::function< void(Item const&) > const& on_select_ = nullptr,
			Menu::ITEM_TYPE type_ = Menu::ITEM_TYPE::OPTION
		) : pngView(pngView_), pngView_selected(pngView_selected_), type(type_), on_select(on_select_) {
		}
		PngView* pngView;
		PngView* pngView_selected;
		Menu::ITEM_TYPE type;
		std::function< void(Item const&) > on_select; //if set, item is selectable
	};


	// ----- Story graph -----
	//story node
	struct SNode {
		std::vector<Item> items = std::vector<Item>();
		uint32_t selected = 0;
	};
	SNode* current_sNode;
	std::vector<SNode*> sNodes = std::vector<SNode*>((size_t)MENUS::MAX);


	// Sets the current story node
	void setSNode(SNode* new_SNode) {
		current_sNode = new_SNode;
		//select first item which can be selected:
		for (uint32_t i = 0; i < current_sNode->items.size(); ++i) {
			if (current_sNode->items[i].on_select) {
				current_sNode->selected = i;
				break;
			}
		}
	}


	// Disables (hides) the menu
	void disableMenu() {
		current_sNode = nullptr;
	}


	// Go up (1) or down (-1) in the menu
	void select_up_down(int incr) {
		if (current_sNode == nullptr) return;
		do {
			int selected = (int)current_sNode->selected;
			selected += incr;
			if (selected < 0) selected = (int)current_sNode->items.size() - 1;
			if (selected >= (int)current_sNode->items.size()) selected = 0;
			current_sNode->selected = (uint32_t)selected;
		} while (current_sNode->items[current_sNode->selected].on_select == nullptr);
	}
};
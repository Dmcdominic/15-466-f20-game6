#include "Menu.hpp"
#include "PngHelper.hpp"


// Constructor
Menu::Menu(PngHelper* pngHelper) {
  // SNode declarations
  SNode* mainMenu = new SNode();
  sNodes.at((int)MENUS::MAIN_MENU) = mainMenu;

  SNode* credits = new SNode();
  sNodes.at((int)MENUS::CREDITS) = credits;

  SNode* pause_lvl = new SNode();
  sNodes.at((int)MENUS::PAUSE_LVL) = pause_lvl;

  SNode* pause_OW = new SNode();
  sNodes.at((int)MENUS::PAUSE_OW) = pause_OW;

  // Starting SNode is mainMenu
  current_sNode = mainMenu;

  // Main Menu
  mainMenu->items.push_back(Item(pngHelper->png_mainMenu_Play, pngHelper->png_mainMenu_Play_selected));
  mainMenu->items.back().on_select = [this](Menu::Item const&) {
    this->play = true;
  };

  mainMenu->items.push_back(Item(pngHelper->png_mainMenu_NewGame, pngHelper->png_mainMenu_NewGame_selected));
  mainMenu->items.back().on_select = [this](Menu::Item const&) {
    this->new_game = true;
  };

  mainMenu->items.push_back(Item(pngHelper->png_mainMenu_Credits, pngHelper->png_mainMenu_Credits_selected));
  mainMenu->items.back().on_select = [this, credits](Menu::Item const&) {
    this->setSNode(credits);
  };

  mainMenu->items.push_back(Item(pngHelper->png_mainMenu_Quit, pngHelper->png_mainMenu_Quit_selected));
  mainMenu->items.back().on_select = [this](Menu::Item const&) {
    this->quit_asap = true;
  };

  // Credits
  credits->items.push_back(Item(pngHelper->png_credits_Credits, nullptr, nullptr, Menu::ITEM_TYPE::IMAGE));

  credits->items.push_back(Item(pngHelper->png_credits_MainMenu, pngHelper->png_credits_MainMenu_selected));
  credits->items.back().on_select = [this, mainMenu](Menu::Item const&) {
    this->setSNode(mainMenu);
  };

  // Pause LVL
  pause_lvl->items.push_back(Item(pngHelper->png_pause_Continue, pngHelper->png_pause_Continue_selected));
  pause_lvl->items.back().on_select = [this](Menu::Item const&) {
    this->disableMenu();
  };

  pause_lvl->items.push_back(Item(pngHelper->png_pause_Restart, pngHelper->png_pause_Restart_selected));
  pause_lvl->items.back().on_select = [this](Menu::Item const&) {
    this->restart_level = true;
  };

  pause_lvl->items.push_back(Item(pngHelper->png_pause_Overworld, pngHelper->png_pause_Overworld_selected));
  pause_lvl->items.back().on_select = [this](Menu::Item const&) {
    this->return_to_OW = true;
  };

  pause_lvl->items.push_back(Item(pngHelper->png_pause_MainMenu, pngHelper->png_pause_MainMenu_selected));
  pause_lvl->items.back().on_select = [this](Menu::Item const&) {
    load_main_menu = true;
  };

  // Pause OW
  pause_OW->items.push_back(Item(pngHelper->png_pause_Continue, pngHelper->png_pause_Continue_selected));
  pause_OW->items.back().on_select = [this](Menu::Item const&) {
    this->disableMenu();
  };

  pause_OW->items.push_back(Item(pngHelper->png_pause_MainMenu, pngHelper->png_pause_MainMenu_selected));
  pause_OW->items.back().on_select = [this](Menu::Item const&) {
    load_main_menu = true;
  };
}
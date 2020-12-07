#include "Menu.hpp"
#include "PngHelper.hpp"


// Constructor
Menu::Menu(PngHelper* pngHelper) {
  // SNode declarations
  SNode* mainMenu = new SNode();
  sNodes.at((int)MENUS::MAIN_MENU) = mainMenu;

  SNode* credits = new SNode();
  sNodes.at((int)MENUS::CREDITS) = credits;

  SNode* pause = new SNode();
  sNodes.at((int)MENUS::PAUSE) = pause;

  // Starting SNode is mainMenu
  current_sNode = mainMenu;

  // Main Menu
  mainMenu->items.push_back(Item(pngHelper->png_mainMenu_Play, pngHelper->png_mainMenu_Play_selected));
  mainMenu->items.back().on_select = [this](Menu::Item const&) {
    //this->disableMenu();
    this->play = true;
  };

  mainMenu->items.push_back(Item(pngHelper->png_mainMenu_NewGame, pngHelper->png_mainMenu_NewGame_selected));
  mainMenu->items.back().on_select = [this](Menu::Item const&) {
    //this->disableMenu();
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

  // Pause
  pause->items.push_back(Item(pngHelper->png_pause_Continue, pngHelper->png_pause_Continue_selected));
  pause->items.back().on_select = [this](Menu::Item const&) {
    this->disableMenu();
  };

  pause->items.push_back(Item(pngHelper->png_pause_Restart, pngHelper->png_pause_Restart_selected));
  pause->items.back().on_select = [this](Menu::Item const&) {
    //this->disableMenu();
    this->restart_level = true;
  };

  pause->items.push_back(Item(pngHelper->png_pause_Overworld, pngHelper->png_pause_Overworld_selected));
  pause->items.back().on_select = [this](Menu::Item const&) {
    //this->disableMenu();
    this->return_to_OW = true;
  };

  pause->items.push_back(Item(pngHelper->png_pause_MainMenu, pngHelper->png_pause_MainMenu_selected));
  pause->items.back().on_select = [this, mainMenu](Menu::Item const&) {
    this->setSNode(mainMenu);
  };
}
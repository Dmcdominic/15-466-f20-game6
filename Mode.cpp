#include "Mode.hpp"

std::shared_ptr< Mode > Mode::current;
std::shared_ptr< Mode > Mode::menu;
std::shared_ptr< Mode > Mode::play;

void Mode::set_current(std::shared_ptr< Mode > const &new_current) {
	current = new_current;
	//NOTE: may wish to, e.g., trigger resize events on new current mode.
}

void Mode::switch_to_menu() {
    current = menu;
}

void Mode::set_menu(std::shared_ptr< Mode > const &d) {
    menu = d;
    current = menu;
}

void Mode::set_play(std::shared_ptr< Mode > const &p) {
    play = p;
    current = play;
}

void Mode::switch_to_play() {
    current = play;
}
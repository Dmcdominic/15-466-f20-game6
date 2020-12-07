#pragma once

#include "PngView.hpp"
#include <glm/glm.hpp>
#include "Menu.hpp"


struct PngHelper {
	PngHelper();
	virtual ~PngHelper();

	private:
		int left[3] = {0, 1, 3};
		int right[3] = {2, 4, 5};
		int top[3] = {0, 3, 5};
		int bottom[3] = {1, 2, 4};

		                  /* lower left triangle */ /* upper right triangle */
		float meter_xs[6]   = {-1.0f, -1.0f, -0.7f, -1.0f, -0.7f, -0.7f};
		float meter_ys[6]   = {-0.5f, -1.0f, -1.0f, -0.5f, -1.0f, -0.5f};
		float barrel_xs1[6] = {-1.0f, -1.0f, -0.8f, -1.0f, -0.8f, -0.8f};
		float barrel_xs2[6] = {-0.8f, -0.8f, -0.6f, -0.8f, -0.6f, -0.6f};
		float barrel_xs3[6] = {-0.6f, -0.6f, -0.4f, -0.6f, -0.4f, -0.4f};
		float barrel_ys[6]  = { 1.0f,  0.8f,  0.8f,  1.0f,  0.8f,  1.0f};
		float wasd_ys[6]    = {-0.2f, -0.9f, -0.9f, -0.2f, -0.9f, -0.2f};
		float enter_xs[6]   = {0.63f, 0.63f, 0.97f, 0.63f, 0.97f, 0.97f};
		float enter_ys[6]   = { 1.0f,  0.7f,  0.7f,  1.0f,  0.7f,  1.0f};
		float reset_ys[6]   = { 0.2f, -0.3f, -0.3f,  0.2f, -0.3f,  0.2f};
		float undo_ys[6]    = { 0.3f, -0.2f, -0.2f,  0.3f, -0.2f,  0.3f};
		float keys_xs[6]    = { 0.7f,  0.7f,  1.0f,  0.7f,  1.0f,  1.0f};
		float sel_inst_xs[6]= { 0.3f,  0.3f,  0.7f,  0.3f,  0.7f,  0.7f};
		float inst_ys[6]    = { 1.0f,  0.5f,  0.5f,  1.0f,  0.5f,  1.0f};
		float bar_inst_xs[6]= {-0.8f, -0.8f, -0.4f, -0.8f, -0.4f, -0.4f};
		float clr_inst_xs[6]= {-0.7f, -0.7f, -0.3f, -0.7f, -0.3f, -0.3f};
		float clr_inst_ys[6]= { 0.2f, -0.3f, -0.3f,  0.2f, -0.3f,  0.2f};
		float clear_xs[6]   = {-0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f};
		float clear_ys[6]   = { 0.7f, -0.7f, -0.7f,  0.7f, -0.7f,  0.7f};

		float bckgrnd_xs[6] = {-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f };
		float bckgrnd_ys[6] = { 1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f };

		float menu_xs[6]    = { -0.2f, -0.2f,  0.2f, -0.2f,  0.2f,  0.2f };
		float menu0_ys[6]   = {  0.3f, 0.15f, 0.15f,  0.3f, 0.15f,  0.3f };
		float menu1_ys[6]   = {  0.1f,-0.05f,-0.05f,  0.1f,-0.05f,  0.1f };
		float menu2_ys[6]   = { -0.1f,-0.25f,-0.25f, -0.1f,-0.25f, -0.1f };
		float menu3_ys[6]   = { -0.7f,-0.85f,-0.85f, -0.7f,-0.85f, -0.7f };
		
		float credits_xs[6] = {-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f };
		float credits_ys[6] = { 0.9f, -0.5f, -0.5f,  0.9f, -0.5f,  0.9f };

		PngView *png_meter100;
		PngView *png_meter75;
		PngView *png_meter50;
		PngView *png_meter25;
		PngView *png_meter0;
		PngView *png_meter;
		PngView *png_meters[5];

		PngView *png_barrel1;
		PngView *png_barrel2;
		PngView *png_barrel3;
		PngView *png_barrels[3];

		PngView *png_wasd;
		PngView *png_reset;
		PngView *png_undo;

		PngView *png_return;
		PngView *png_select;

		PngView *png_sel_inst;
		PngView *png_bar_inst;
		PngView *png_ret_inst;
		PngView *png_clr_inst;

		PngView *png_clear;

		PngView* png_main_background;
		PngView* png_credits_background;
		PngView* png_pause_background;

	public:
		PngView* png_mainMenu_Play;
		PngView* png_mainMenu_Play_selected;
		PngView* png_mainMenu_NewGame;
		PngView* png_mainMenu_NewGame_selected;
		PngView* png_mainMenu_Credits;
		PngView* png_mainMenu_Credits_selected;
		PngView* png_mainMenu_Quit;
		PngView* png_mainMenu_Quit_selected;

		PngView* png_credits_Credits;
		PngView* png_credits_MainMenu;
		PngView* png_credits_MainMenu_selected;

		PngView* png_pause_Continue;
		PngView* png_pause_Continue_selected;
		PngView* png_pause_Restart;
		PngView* png_pause_Restart_selected;
		PngView* png_pause_Overworld;
		PngView* png_pause_Overworld_selected;
		PngView* png_pause_MainMenu;
		PngView* png_pause_MainMenu_selected;

		PngView *png_buttons[18];

	private:
		// for barrels
		int already_disposed = 0;
		int barrels_drawn = 0;

		// animated main menu background
		int menu_frame = 0;
		int menu_frames = 7;
		int frame_count = 0;
		int frame_goal = 9;

		glm::uvec2 prev_drawable_size = glm::uvec2(0, 0);

	public:
		int drawing_level = 0;

		void update_env_score(int score);
		void update_png_pos(glm::uvec2 const &drawable_size);
		void reset();
		void draw();
		void draw(bool draw_barrel, bool draw_wasd, bool draw_return, bool draw_select, bool draw_reset,
				int num_disposed, int goal, int cur_level, Menu *menu);
};

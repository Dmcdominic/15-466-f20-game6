#pragma once

#include "PngView.hpp"
#include <glm/glm.hpp>

#define NUM_PNG 6

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
		float wasd_ys[6]    = {-0.3f, -0.9f, -0.9f, -0.3f, -0.9f, -0.3f};
		float enter_xs[6]   = {0.63f, 0.63f, 0.97f, 0.63f, 0.97f, 0.97f};
		float enter_ys[6]   = { 1.0f,  0.7f,  0.7f,  1.0f,  0.7f,  1.0f};
		float reset_ys[6]   = { 0.2f, -0.3f, -0.3f,  0.2f, -0.3f,  0.2f};
		float undo_ys[6]    = { 0.3f, -0.2f, -0.2f,  0.3f, -0.2f,  0.3f};
		float keys_xs[6]    = { 0.7f,  0.7f,  1.0f,  0.7f,  1.0f,  1.0f};
		float sel_inst_xs[6]= { 0.3f,  0.3f,  0.7f,  0.3f,  0.7f,  0.7f};
		float inst_ys[6]    = { 1.0f,  0.5f,  0.5f,  1.0f,  0.5f,  1.0f};
		float bar_inst_xs[6]= {-0.8f, -0.8f, -0.4f, -0.8f, -0.4f, -0.4f};

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

		// for barrels
		int already_disposed = 0;
		int barrels_drawn = 0;

		glm::uvec2 prev_drawable_size = glm::uvec2(0, 0);

	public:
		int drawing_level = 0;

		void update_env_score(int score);
		void update_png_pos(glm::uvec2 const &drawable_size);
		void reset();
		void draw();
		void draw(bool draw_barrel, bool draw_wasd, bool draw_return, bool draw_select, bool draw_reset,
				int num_disposed, int goal, int cur_level);
};

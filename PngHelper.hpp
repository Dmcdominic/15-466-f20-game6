#pragma once

#include "PngView.hpp"
#include <glm/glm.hpp>

struct PngHelper {
	PngHelper();

	int left[3] = {0, 1, 3};
	int right[3] = {2, 4, 5};
	int top[3] = {0, 3, 5};
	int bottom[3] = {1, 2, 4};

				    /* lower left triangle */  /* upper right triangle */
	float meter_xs[6] = {-1.0f, -1.0f, -0.7f, -1.0f, -0.7f, -0.7f};
	float meter_ys[6] = {-0.5f, -1.0f, -1.0f, -0.5f, -1.0f, -0.5f};
	float barrel_xs[6] = {-1.0f, -1.0f, -0.7f, -1.0f, -0.7f, -0.7f};
	float barrel_ys[6] = {1.0f, 0.7f, 0.7f, 1.0f, 0.7f, 1.0f};
	float wasd_xs[6] = {0.7f, 0.7f, 1.0f, 0.7f, 1.0f, 1.0f};
	float wasd_ys[6] = {-0.45f, -0.9f, -0.9f, -0.45f, -0.9f, -0.45f};

	PngView *png_meter100;
	PngView *png_meter75;
	PngView *png_meter50;
	PngView *png_meter25;
	PngView *png_meter0;
	PngView *png_meter;
	PngView *png_barrel;
	PngView *png_wasd;


	glm::uvec2 prev_drawable_size = glm::uvec2(0, 0);

	void update_env_score(int score);
	void update_png_pos(glm::uvec2 const &drawable_size);
	void draw(bool draw_barrel, bool draw_wasd);
};

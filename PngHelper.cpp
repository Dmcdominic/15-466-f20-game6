#include "PngHelper.hpp"

#include <math.h>
#include <iostream>

PngHelper::PngHelper() {

	png_meter100 = new PngView("meter100.png", meter_xs, meter_ys);
	png_meter75 = new PngView("meter75.png", meter_xs, meter_ys);
	png_meter50 = new PngView("meter50.png", meter_xs, meter_ys);
	png_meter25 = new PngView("meter25.png", meter_xs, meter_ys);
	png_meter0 = new PngView("meter0.png", meter_xs, meter_ys);
	png_meter = png_meter0;

	png_meters[0] = png_meter0;
	png_meters[1] = png_meter25;
	png_meters[2] = png_meter50;
	png_meters[3] = png_meter75;
	png_meters[4] = png_meter100;

	png_barrel1 = new PngView("barrel_faded.png", barrel_xs1, barrel_ys);
	png_barrel2 = new PngView("barrel_faded.png", barrel_xs2, barrel_ys);
	png_barrel3 = new PngView("barrel_faded.png", barrel_xs3, barrel_ys);

	png_barrels[0] = png_barrel1;
	png_barrels[1] = png_barrel2;
	png_barrels[2] = png_barrel3;

	png_wasd = new PngView("wasd.png", wasd_xs, wasd_ys);
	png_enter = new PngView("enter.png", enter_xs, enter_ys);
	png_reset = new PngView("reset.png", reset_xs, reset_ys);
	png_undo = new PngView("undo.png", undo_xs, undo_ys);

}

PngHelper::~PngHelper() {
}

void PngHelper::update_env_score(int score) {
	png_meter = png_meters[int((100 - score)/25)];
}

void PngHelper::update_png_pos(glm::uvec2 const &drawable_size) {
	// only load new positions if window size changed
	if (prev_drawable_size != drawable_size) {
		if (prev_drawable_size == glm::uvec2(0, 0)) {
			prev_drawable_size = drawable_size;
		}
		float prev_area = float(prev_drawable_size.x) * float(prev_drawable_size.y);
		float cur_area = float(drawable_size.x) * float(drawable_size.y);

		float barrel_w = (barrel_xs1[2] - barrel_xs1[1]) * prev_drawable_size.x;
		float barrel_x = sqrt((barrel_w * barrel_w / prev_area) * cur_area) / drawable_size.x;
		float barrel_y = barrel_x * drawable_size.x / drawable_size.y;

		float meter_w = (meter_xs[2] - meter_xs[1]) * prev_drawable_size.x;
		float meter_h = (meter_ys[0] - meter_ys[1]) * prev_drawable_size.y;
		float meter_x = sqrt((meter_w * meter_h / prev_area) * cur_area) / drawable_size.x;
		float meter_y = meter_x * drawable_size.x / drawable_size.y;

		float wasd_w = (wasd_xs[2] - wasd_xs[1]) * prev_drawable_size.x;
		float wasd_h = (meter_ys[0] - meter_ys[1]) * prev_drawable_size.y;
		float wasd_x = sqrt((wasd_w * wasd_h / prev_area) * cur_area) / drawable_size.x;
		float wasd_y = wasd_x * drawable_size.x / drawable_size.y;

		float enter_w = (enter_xs[2] - enter_xs[1]) * prev_drawable_size.x;
		float enter_h = (enter_ys[0] - enter_ys[1]) * prev_drawable_size.y;
		float enter_x = sqrt((enter_w * enter_h / prev_area) * cur_area) / drawable_size.x;
		float enter_y = enter_x * drawable_size.x / drawable_size.y;

		float reset_w = (reset_xs[2] - reset_xs[1]) * prev_drawable_size.x;
		float reset_h = (reset_ys[0] - reset_ys[1]) * prev_drawable_size.y;
		float reset_x = sqrt((reset_w * reset_h / prev_area) * cur_area) / drawable_size.x;
		float reset_y = reset_x * drawable_size.x / drawable_size.y;

		float undo_w = (undo_xs[2] - undo_xs[1]) * prev_drawable_size.x;
		float undo_h = (undo_ys[0] - undo_ys[1]) * prev_drawable_size.y;
		float undo_x = sqrt((undo_w * undo_h / prev_area) * cur_area) / drawable_size.x;
		float undo_y = undo_x * drawable_size.x / drawable_size.y;

		// update positions according to position on screen
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				png_barrels[j]->xs[right[i]] = barrel_x - 1 + 0.3f * j;
				png_barrels[j]->ys[bottom[i]] = 1 - barrel_y;
			}
			for (int j = 0; j < 5; j++) {
				png_meters[j]->xs[right[i]] = meter_x - 1;
				png_meters[j]->ys[top[i]] = meter_y - 1;
			}
			png_wasd->xs[left[i]] = 1 - wasd_x;
			png_wasd->ys[top[i]] = wasd_y - 1;
			png_enter->xs[left[i]] = 0.97f - enter_x;
			png_enter->ys[bottom[i]] = 1 - enter_y;
			png_reset->xs[left[i]] = 1 - reset_x;
			png_reset->ys[bottom[i]] = png_wasd->ys[top[i]];
			png_reset->ys[top[i]] = png_reset->ys[bottom[i]] + reset_y;
			png_undo->xs[left[i]] = 1 - undo_x;
			png_undo->ys[bottom[i]] = png_reset->ys[top[i]];
			png_undo->ys[top[i]] = png_undo->ys[bottom[i]] + undo_y;
		}

		// load the new vertex positions
		for (int j = 0; j < 3; j++) {
			png_barrels[j]->load();
		}
		for (int j = 0; j < 5; j++) {
			png_meters[j]->load();
		}
		png_wasd->load();
		png_enter->load();
		png_reset->load();
		png_undo->load();

		prev_drawable_size = drawable_size;
	}

}

void PngHelper::draw() {
	// for drawing on level completion
	draw(true, true, true, false, barrels_drawn, barrels_drawn, drawing_level);
}

void PngHelper::draw(bool draw_barrel, bool draw_wasd, bool draw_enter, bool draw_reset,
		int num_disposed, int goal, int cur_level) {
	if (draw_barrel) {
		// set local fields to match current grid
		if (drawing_level != cur_level) {
			for (int i = 0; i < 3; i++) {
				png_barrels[i]->filename = "barrel_faded.png";
				png_barrels[i]->load();
			}
			drawing_level = cur_level;
			barrels_drawn = goal;
			already_disposed = 0;
		}
		if (already_disposed < num_disposed) {
			for (int i = already_disposed; i < num_disposed; i++) {
				png_barrels[i]->filename = "barrel.png";
				png_barrels[i]->load();
			}
			already_disposed = num_disposed;
		}
		for (int i = 0; i < barrels_drawn; i++) {
			png_barrels[i]->draw();
		}
	}
	png_meter->draw();
	if (draw_wasd) png_wasd->draw();
	if (draw_enter) png_enter->draw();
	if (draw_reset) {
		png_reset->draw();
		png_undo->draw();
	}
}
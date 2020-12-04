#include "PngHelper.hpp"

#include <math.h>

PngHelper::PngHelper() {

	png_meter100 = new PngView("meter100.png", meter_xs, meter_ys);
	png_meter75 = new PngView("meter75.png", meter_xs, meter_ys);
	png_meter50 = new PngView("meter50.png", meter_xs, meter_ys);
	png_meter25 = new PngView("meter25.png", meter_xs, meter_ys);
	png_meter0 = new PngView("meter0.png", meter_xs, meter_ys);
	png_meter = png_meter0;

	png_barrel = new PngView("barrel_faded.png", barrel_xs, barrel_ys);
//	png_barrel2 = new PngView("barrel_faded.png", barrel_xs2, barrel_ys2);
//	png_barrel3 = new PngView("barrel_faded.png", barrel_xs3, barrel_ys3);

	png_wasd = new PngView("wasd.png", wasd_xs, wasd_ys);

}

void PngHelper::update_env_score(int score) {
	if (score >= 87) {
		png_meter = png_meter0;
	} else if (score >= 62) {
		png_meter = png_meter25;
	} else if (score >= 37) {
		png_meter = png_meter50;
	} else if (score >= 12) {
		png_meter = png_meter75;
	} else {
		png_meter = png_meter100;
	}
}

void PngHelper::update_png_pos(glm::uvec2 const &drawable_size) {
	if (prev_drawable_size != drawable_size) {
		if (prev_drawable_size == glm::uvec2(0, 0)) {
			prev_drawable_size = drawable_size;
		}
		float prev_area = float(prev_drawable_size.x) * float(prev_drawable_size.y);
		float cur_area = float(drawable_size.x) * float(drawable_size.y);

		float barrel_w = (1 + barrel_xs[5]) * prev_drawable_size.x;
		float barrel_x = sqrt((barrel_w * barrel_w / prev_area) * cur_area) / drawable_size.x;
		float barrel_y = barrel_x * drawable_size.x / drawable_size.y;

		float meter_w = (1 + meter_xs[5]) * prev_drawable_size.x;
		float meter_h = (1 + meter_ys[5]) * prev_drawable_size.y;
		float meter_x = sqrt((meter_w * meter_h / prev_area) * cur_area) / drawable_size.x;
		float meter_y = meter_x * drawable_size.x / drawable_size.y;

		float wasd_w = (1 - wasd_xs[0]) * prev_drawable_size.x;
		float wasd_h = (1 + (wasd_ys[1] - wasd_ys[0])) * prev_drawable_size.y;
		float wasd_x = sqrt((wasd_w * wasd_h / prev_area) * cur_area) / drawable_size.x;
		float wasd_y = wasd_x * drawable_size.x / drawable_size.y;

		for (int i = 0; i < 3; i++) {
			png_barrel->xs[right[i]] = barrel_x - 1;
			png_barrel->ys[bottom[i]] = 1 - barrel_y;
			png_meter->xs[right[i]] = meter_x - 1;
			png_meter->ys[top[i]] = meter_y - 1;
			png_wasd->xs[left[i]] = 1 - wasd_x;
			png_wasd->ys[top[i]] = wasd_y - 1;
		}

		png_barrel->load();
		png_meter->load();
		png_wasd->load();

		prev_drawable_size = drawable_size;
	}

}

void PngHelper::draw(bool draw_barrel, bool draw_wasd) {
	png_meter->draw();
	if (draw_barrel) png_barrel->draw();
	if (draw_wasd) png_wasd->draw();
}
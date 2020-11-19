// written by Xiaoqiao Xu.

#pragma once

#include "GL.hpp"
#include <string>

class PngView {
public:
	PngView();
	~PngView();

	// copy semantics is disallowed
	// because I don't have time for writing these functions.
	PngView(const PngView &) = delete;
	PngView& operator=(const PngView &) = delete;

	std::string meter_filename = "pingu.png";

	void draw();
private:
	GLuint vao_ = 0;
	GLuint vbo_ = 0;
	GLuint texture_ = 0;
};
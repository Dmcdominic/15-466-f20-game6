#include "PngView.hpp"
#include "ColorTextureProgram.hpp"
#include "data_path.hpp"
#include "load_save_png.hpp"
#include <glm/glm.hpp>
#include "gl_errors.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <vector>


PngView::PngView(std::string _filename, float *_xs, float *_ys) {
	filename = _filename;
	xs = _xs;
	ys = _ys;
	load();
}

PngView::~PngView() {
	glDeleteBuffers(1, &vbo_);
	glDeleteVertexArrays(1, &vao_);
	glDeleteTextures(1, &texture_);
	GL_ERRORS();
}

void PngView::load() {
	// vertex attributes for the "quad"
	float quad_vertices[] = {
			// positions vec4         // colors vec4          // texCoords vec2
			xs[0], ys[0], 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			xs[1], ys[1], 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			xs[2], ys[2], 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

			xs[3], ys[3], 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			xs[4], ys[4], 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			xs[5], ys[5], 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
	};
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	// GL_STATIC_DRAW: The data store contents will be modified once and used many times as the source for GL drawing commands.
	// GL_DYNAMIC_DRAW: The data store contents will be modified repeatedly and used many times as the source for GL drawing commands.
	// https://www.khronos.org/registry/OpenGL-Refpages/es1.1/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);
	glEnableVertexAttribArray(color_texture_program->Position_vec4_2);
	glVertexAttribPointer(color_texture_program->Position_vec4_2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(color_texture_program->Color_vec4_2);
	glVertexAttribPointer(color_texture_program->Color_vec4_2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *) (4 * sizeof(float)));
	glEnableVertexAttribArray(color_texture_program->TexCoord_vec2_2);
	glVertexAttribPointer(color_texture_program->TexCoord_vec2_2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *) (8 * sizeof(float)));

	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_2D, texture_);

	glm::uvec2 png_size;
	std::vector<glm::u8vec4> data;
	load_png(data_path("../pngs/" + filename), &png_size, &data, LowerLeftOrigin);
	glTexImage2D(GL_TEXTURE_2D,
	             0, GL_RGBA, png_size.x, png_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data()
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	GL_ERRORS();
}

void PngView::draw() {
	glm::mat4 identity_matrix(1.0f);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(color_texture_program->program2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glBindVertexArray(vao_);

	glUniformMatrix4fv(color_texture_program->OBJECT_TO_CLIP_mat4_2, 1, GL_FALSE, glm::value_ptr(identity_matrix));
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	GL_ERRORS();
}
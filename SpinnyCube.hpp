
#include "Renderer.hpp"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>

#ifndef SPINNYCUBE_HPP
#define SPINNYCUBE_HPP

class SpinnyCube : public Renderer {
	public:

		GLuint vao;
		GLuint vbo;
		GLint mv_location;
		GLint proj_location;

		SpinnyCube();
		void startup();
		void render(double time);
		void finish();
};

#endif
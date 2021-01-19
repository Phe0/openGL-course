#include "Renderer.hpp"
#include <cmath>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>

#ifndef SQUARE_HPP
#define SQUARE_HPP

class Square : public Renderer {
public:

	GLuint vao;
	GLuint vbo;
	GLuint uniformModel;

	Square();
	void startup();
	void render(double time);
	void finish();
	void createSquare();
	void compileShaders();
};

#endif
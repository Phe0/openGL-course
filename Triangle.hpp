#include "Renderer.hpp"

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

class Triangle : public Renderer {
public:

	GLuint vao;
	GLuint vbo;

	Triangle();
	void startup();
	void render(double time);
	void finish();
	void createTriangle();
	void compileShaders();
};

#endif
#include "Renderer.hpp"
#include <cmath>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

class Triangle : public Renderer {
public:

	GLuint vao;
	GLuint vbo;
	GLuint ibo;
	GLuint uniformModel;
	GLuint uniformProjection;

	glm::mat4 projection;

	Triangle();
	void startup();
	void render(double time);
	void finish();
	void createTriangle();
	void compileShaders();
};

#endif
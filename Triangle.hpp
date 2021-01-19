#include "Renderer.hpp"
#include <cmath>
#include <vector>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>

#include "Mesh.hpp"

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

class Triangle : public Renderer {
public:

	GLuint uniformModel;
	GLuint uniformProjection;
	std::vector<Mesh*> meshList;

	glm::mat4 projection;

	Triangle();
	void startup();
	void render(double time);
	void finish();
	void createTriangle();
	void compileShaders();
};

#endif
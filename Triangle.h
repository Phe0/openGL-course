#include "Renderer.h"
#include <cmath>
#include <vector>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "CommonValues.h"
#include "PointLight.h"

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

class Triangle : public Renderer {
public:

	GLuint uniformModel;
	GLuint uniformProjection;
	GLuint uniformView;
	GLuint uniformEyePosition;
	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;

	Texture brickTexture;
	Texture dirtTexture;
	Texture plainTexture;

	Material shinyMaterial;
	Material dullMaterial;

	DirectionalLight mainLight;
	PointLight pointLights[MAX_POINT_LIGHTS];

	unsigned int pointLightCount;

	std::vector<Mesh*> meshList;
	std::vector<Shader> shaderList;

	glm::mat4 projection;

	Triangle();
	void startup();
	void render(double time);
	void finish();
	void createTriangle();
	void createShaders();

	void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticesCount, unsigned int vLength, unsigned int normalOffset);
};

#endif
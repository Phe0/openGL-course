#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>

#include <GL/glew.h>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "CommonValues.h"

class Shader {
public:
	Shader();

	void createFromString(const char* vertexCode, const char* fragmentCode);
	void createFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string readfile(const char* fileLocation);

	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	GLuint getAmbientIntensityLocation();
	GLuint getAmbientColorLocation();
	GLuint getDiffuseIntensityLocation();
	GLuint getDirectionLocation();
	GLuint getSpecularIntensityLocation();
	GLuint getShininessLocation();
	GLuint getEyePositionLocation();

	void setDirectionLight(DirectionalLight * dLight);
	void setPointLights(PointLight * pLight, unsigned int lightCount);

	void useShader();
	void clearShader();

	~Shader();

private:
	int pointLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition, uniformSpecularIntensity, uniformShininess;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLenum type, const char* source);
};
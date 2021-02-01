#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>

#include <GL/glew.h>

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

	void useShader();
	void clearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection;

	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLenum type, const char* source);
};
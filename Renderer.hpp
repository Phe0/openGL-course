#include <string.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef RENDERER_HPP
#define RENDERER_HPP

class Renderer {

public:
	GLuint width;
	GLuint height;
	GLuint program;
	int bufferWidth;
	int bufferHeight;

	Renderer();
	int runProject();
	void addShader(GLenum type, const char* source);

protected:
	virtual void startup() = 0;
	virtual void render(double time) = 0;
	virtual void finish() = 0;
};

#endif
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"

#ifndef RENDERER_HPP
#define RENDERER_HPP

class Renderer {

public:
	GLuint width;
	GLuint height;
	GLuint program;
	int bufferWidth;
	int bufferHeight;
	GLFWwindow* mainWindow;
	Camera camera;

	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0f;

	Renderer();
	int runProject();

	bool keys[1024];

	GLfloat lastx;
	GLfloat lasty;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	GLfloat getXChange();
	GLfloat getYChange();

	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

protected:
	virtual void startup() = 0;
	virtual void render(double time) = 0;
	virtual void finish() = 0;
};

#endif
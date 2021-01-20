#include "Renderer.hpp"


Renderer::Renderer() {
	width = 700;
	height = 400;
	program = 0;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}

	xChange = 0.0f;
	yChange = 0.0f;
}

int Renderer::runProject() {

	if (!glfwInit()) {
		printf("GLFW initialization failed");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	this->mainWindow = glfwCreateWindow(this->width, this->height, "Test Window", NULL, NULL);

	if (!this->mainWindow) {
		printf("GLFW window creation failed");
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(this->mainWindow, &this->bufferWidth, &this->bufferHeight);

	glfwMakeContextCurrent(this->mainWindow);

	this->createCallbacks();
	glfwSetInputMode(this->mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;


	if (glewInit() != GLEW_OK) {
		printf("GLEW initialization failed");
		glfwDestroyWindow(this->mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, this->bufferWidth, this->bufferHeight);

	glfwSetWindowUserPointer(this->mainWindow, this);

	this->startup();

	while (!glfwWindowShouldClose(this->mainWindow)) {
		glfwPollEvents();

		GLfloat now = glfwGetTime();

		this->deltaTime = now - this->lastTime;
		this->lastTime = now;

		this->render(now);

		glfwSwapBuffers(this->mainWindow);
	}

	this->finish();

	glfwTerminate();

	return 0;
}

void Renderer::createCallbacks() {
	glfwSetKeyCallback(this->mainWindow, this->handleKeys);
	glfwSetCursorPosCallback(this->mainWindow, this->handleMouse);
}

void Renderer::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
	Renderer* theRenderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			theRenderer->keys[key] = true; \
		}
		else if (action == GLFW_RELEASE) {
			theRenderer->keys[key] = false;
		}
	}
}

void Renderer::handleMouse(GLFWwindow* window, double xPos, double yPos) {
	Renderer* theRenderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));

	if (theRenderer->mouseFirstMoved) {
		theRenderer->lastx = xPos;
		theRenderer->lasty = yPos;
		theRenderer->mouseFirstMoved = false;
	}

	theRenderer->xChange = xPos - theRenderer->lastx;
	theRenderer->yChange = theRenderer->lasty - yPos;

	theRenderer->lastx = xPos;
	theRenderer->lasty = yPos;
}

GLfloat Renderer::getXChange() {
	GLfloat theChange = this->xChange;
	this->xChange = 0.0f;
	return theChange;
}

GLfloat Renderer::getYChange() {
	GLfloat theChange = this->yChange;
	this->yChange = 0.0f;
	return theChange;
}
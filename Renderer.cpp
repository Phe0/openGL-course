#include "Renderer.hpp"


Renderer::Renderer() {
	width = 700;
	height = 400;
	program = 0;
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


	GLFWwindow* mainWindow = glfwCreateWindow(this->width, this->height, "Test Window", NULL, NULL);

	if (!mainWindow) {
		printf("GLFW window creation failed");
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(mainWindow, &this->bufferWidth, &this->bufferHeight);

	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE;


	if (glewInit() != GLEW_OK) {
		printf("GLEW initialization failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, this->bufferWidth, this->bufferHeight);

	this->startup();

	while (!glfwWindowShouldClose(mainWindow)) {
		glfwPollEvents();

		this->render(glfwGetTime());

		glfwSwapBuffers(mainWindow);
	}

	this->finish();

	glfwTerminate();

	return 0;
}

void Renderer::addShader(GLenum type, const char* source) {
	GLuint shader = glCreateShader(type);

	const GLchar* theCode[1];
	theCode[0] = source;

	GLint codeLength[1];
	codeLength[0] = strlen(source);

	glShaderSource(shader, 1, theCode, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Erro compiling the %d shader: '%s'\n", type, eLog);
		return;
	}

	glAttachShader(this->program, shader);
}

#include "Shader.h"

Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode) {
	this->compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char* vertexLocation, const char* fragmentLocation) {
    std::string vertexString = this->readfile(vertexLocation);
    std::string fragmentString = this->readfile(fragmentLocation);
    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    this->compileShader(vertexCode, fragmentCode);
}

std::string Shader::readfile(const char* fileLocation) {
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open()) {
        printf("Failed to read %s| File doesnt exist.", fileLocation);
        return "";
    }

    std::string line = "";

    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

void Shader::compileShader(const char* vertexCode, const char* fragmentCode) {
    this->shaderID = glCreateProgram();

    if (!this->shaderID) {
        printf("Erro creating shader program!\n");
        return;
    }

    this->addShader(GL_VERTEX_SHADER, vertexCode);
    this->addShader(GL_FRAGMENT_SHADER, fragmentCode);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(this->shaderID);
    glGetProgramiv(this->shaderID, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(this->shaderID, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(this->shaderID);
    glGetProgramiv(this->shaderID, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(this->shaderID, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

    this->uniformModel = glGetUniformLocation(this->shaderID, "model");
    this->uniformProjection = glGetUniformLocation(this->shaderID, "projection");
    this->uniformView = glGetUniformLocation(this->shaderID, "view");
    this->uniformAmbientColor = glGetUniformLocation(this->shaderID, "directionalLight.color");
    this->uniformAmbientIntensity = glGetUniformLocation(this->shaderID, "directionalLight.ambientIntensity");
}

GLuint Shader::getProjectionLocation() {
    return this->uniformProjection;
}

GLuint Shader::getModelLocation() {
    return this->uniformModel;
}

GLuint Shader::getViewLocation() {
    return this->uniformView;
}

GLuint Shader::getAmbientColorLocation() {
    return this->uniformAmbientColor;
}

GLuint Shader::getAmbientIntensityLocation() {
    return this->uniformAmbientIntensity;
}

void Shader::useShader() {
    glUseProgram(this->shaderID);
}

void Shader::clearShader() {
    if (this->shaderID != 0) {
        glDeleteProgram(this->shaderID);
        this->shaderID = 0;
    }
}


void Shader::addShader(GLenum type, const char* source) {
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

	glAttachShader(this->shaderID, shader);
}

Shader::~Shader() {
    this->clearShader();
}
#include "Shader.h"

Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
    pointLightCount = 0;
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

    this->uniformProjection = glGetUniformLocation(this->shaderID, "projection");
    this->uniformModel = glGetUniformLocation(this->shaderID, "model");
    this->uniformView = glGetUniformLocation(this->shaderID, "view");
    this->uniformDirectionalLight.uniformColor = glGetUniformLocation(this->shaderID, "directionalLight.base.color");
    this->uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(this->shaderID, "directionalLight.base.ambientIntensity");
    this->uniformDirectionalLight.uniformDirection = glGetUniformLocation(this->shaderID, "directionalLight.direction");
    this->uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(this->shaderID, "directionalLight.base.diffuseIntensity");
    this->uniformSpecularIntensity = glGetUniformLocation(this->shaderID, "material.specularIntensity");
    this->uniformShininess = glGetUniformLocation(this->shaderID, "material.shininess");
    this->uniformEyePosition = glGetUniformLocation(this->shaderID, "eyePosition");

    this->uniformPointLightCount = glGetUniformLocation(this->shaderID, "pointLightCount");

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
        this->uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
        this->uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(this->shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        this->uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(this->shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        this->uniformPointLight[i].uniformPosition = glGetUniformLocation(this->shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        this->uniformPointLight[i].uniformConstant = glGetUniformLocation(this->shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        this->uniformPointLight[i].uniformLinear = glGetUniformLocation(this->shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        this->uniformPointLight[i].uniformExponent = glGetUniformLocation(this->shaderID, locBuff);
    }
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
    return this->uniformDirectionalLight.uniformColor;
}

GLuint Shader::getAmbientIntensityLocation() {
    return this->uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::getDirectionLocation() {
    return this->uniformDirectionalLight.uniformDirection;
}

GLuint Shader::getDiffuseIntensityLocation() {
    return this->uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::getSpecularIntensityLocation() {
    return this->uniformSpecularIntensity;
}

GLuint Shader::getShininessLocation() {
    return this->uniformShininess;
}

GLuint Shader::getEyePositionLocation() {
    return this->uniformEyePosition;
}

void Shader::setDirectionLight(DirectionalLight * dLight) {
    dLight->useLight(this->uniformDirectionalLight.uniformAmbientIntensity, this->uniformDirectionalLight.uniformColor, this->uniformDirectionalLight.uniformDiffuseIntensity, this->uniformDirectionalLight.uniformDirection);
};

void Shader::setPointLights(PointLight * pLight, unsigned int lightCount) {
    if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

    glUniform1i(this->uniformPointLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++) {
        pLight[i].useLight(this->uniformPointLight[i].uniformAmbientIntensity, this->uniformPointLight[i].uniformColor, this->uniformPointLight[i].uniformDiffuseIntensity, this->uniformPointLight[i].uniformPosition, this->uniformPointLight[i].uniformConstant, this->uniformPointLight[i].uniformLinear, this->uniformPointLight[i].uniformExponent);
    }
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
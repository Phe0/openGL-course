#include "PointLight.h"

PointLight::PointLight() : Light() {
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->constant = 1.0f;
    this->linear = 0.0f;
    this->exponent = 0.0f;
};

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
                      GLfloat aIntensity, GLfloat dIntensity,
                      GLfloat xPos, GLfloat yPos, GLfloat zPos,
                      GLfloat con, GLfloat lin, GLfloat exp) :
                      Light(red, green, blue, aIntensity, dIntensity)
{
    this->position = glm::vec3(xPos, yPos, zPos);
    this->constant = con;
    this->linear = lin;
    this->exponent = exp;
}

void PointLight::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation) {
	glUniform3f(ambientColorLocation, this->color.x, this->color.y, this->color.z);
	glUniform1f(ambientIntensityLocation, this->ambientIntensity);
	glUniform1f(diffuseIntensityLocation, this->diffuseIntensity);

	glUniform3f(positionLocation, this->position.x, this->position.y, this->position.z);
	glUniform1f(constantLocation, this->constant);
	glUniform1f(linearLocation, this->linear);
	glUniform1f(exponentLocation, this->exponent);
}

PointLight::~PointLight() {

}
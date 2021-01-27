#include "Light.h"

Light::Light() {
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity) {
	this->color = glm::vec3(red, green, blue);
	this->ambientIntensity = aIntensity;
}

void Light::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation) {
	glUniform3f(ambientColorLocation, this->color.x, this->color.y, this->color.z);
	glUniform1f(ambientIntensityLocation, this->ambientIntensity);
}

Light::~Light() {

}
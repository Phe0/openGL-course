#include "Light.h"

Light::Light() {
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity) {
	this->color = glm::vec3(red, green, blue);
	this->ambientIntensity = aIntensity;

	this->direction = glm::vec3(xDir, yDir, zDir);
	this->diffuseIntensity = dIntensity;
}

void Light::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation) {
	glUniform3f(ambientColorLocation, this->color.x, this->color.y, this->color.z);
	glUniform1f(ambientIntensityLocation, this->ambientIntensity);

	glUniform3f(directionLocation, this->direction.x, this->direction.y, this->direction.z);
	glUniform1f(diffuseIntensityLocation, this->diffuseIntensity);
}

Light::~Light() {

}
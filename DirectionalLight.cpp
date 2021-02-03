#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
                                  GLfloat aIntensity, GLfloat dIntensity,
                                  GLfloat xDir, GLfloat yDir, GLfloat zDir) :
                                  Light(red, green, blue, aIntensity, dIntensity)
{
	this->direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation) {
	glUniform3f(ambientColorLocation, this->color.x, this->color.y, this->color.z);
	glUniform1f(ambientIntensityLocation, this->ambientIntensity);

	glUniform3f(directionLocation, this->direction.x, this->direction.y, this->direction.z);
	glUniform1f(diffuseIntensityLocation, this->diffuseIntensity);
}

DirectionalLight::~DirectionalLight() {
    
}
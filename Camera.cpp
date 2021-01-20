#include "Camera.hpp"

Camera::Camera() {

}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed) {
	this->position = startPosition;
	this->worldUp = startUp;
	this->yaw = startYaw;
	this->pitch = startPitch;
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);

	this->movementSpeed = startMovementSpeed;
	this->turnSpeed = startTurnSpeed;

	this->update();
}

void Camera::update() {
	this->front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front.y = sin(glm::radians(pitch));
	this->front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	this->front = glm::normalize(front);

	this->right = glm::normalize(glm::cross(this->front, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) {

	GLfloat velocity = this->movementSpeed * deltaTime;

	if (keys[GLFW_KEY_W]) {
		this->position += this->front * velocity;
	}
	if (keys[GLFW_KEY_S]) {
		this->position -= this->front * velocity;
	}
	if (keys[GLFW_KEY_A]) {
		this->position -= this->right * velocity;
	}
	if (keys[GLFW_KEY_D]) {
		this->position += this->right * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= this->turnSpeed;
	yChange *= this->turnSpeed;

	this->yaw += xChange;
	this->pitch += yChange;

	printf("%.6f\n", xChange);
	printf("%.6f\n\n", yChange);

	if (this->pitch > 89.0f) {
		this->pitch = 89.0f;
	}

	if (this->pitch < -89.0f) {
		this->pitch = -89.0f;
	}

	this->update();
}

glm::mat4 Camera::claculateViewMatrix() {
	return glm::lookAt(this->position, this->position + this->front, this->up);
}

Camera::~Camera() {

}
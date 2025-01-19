// standard libraries
#include <iostream>

// additional libraries
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

// project libraries
#include "Camera.hpp" // IMPLEMENTS
#include "globals.hpp"
#include "util.hpp"

const float look_speed_divisor = 5.0;
const float move_speed_divisor = 1.5;

Camera::Camera() {
	std::cout << "Camera.cpp: (Constructor) created a Camera!\n";
	logfile("Camera.cpp: (Constructor) created a Camera!");

	//m_eyePos = glm::vec3(0.0f, 0.0f, 5.0f);
	//m_viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	//m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	m_eyePos = glm::vec3(5.0f, 5.0f, 5.0f);
	m_viewDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
	m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(	m_eyePos,
						m_eyePos + m_viewDirection,
						m_upVector);
}

glm::mat4 Camera::GetProjectionMatrix() const {
	glm::mat4 perspective = glm::perspective(
		glm::radians(45.0f),
		(float)g.gScreenWidth / (float)g.gScreenHeight,
		0.1f,
		// 20.0f // "Default" far plane
		200.0f
	);
	return perspective;
}

void Camera::MouseLook(int mouseX, int mouseY) {
	glm::vec2 newMousePos(mouseX, mouseY);

	static bool firstLook = true;
	if (firstLook) {
		firstLook = false;
		m_oldMousePos = newMousePos;
	}

	float delta_mouse_X = newMousePos.x - m_oldMousePos.x;
	float delta_mouse_Y = newMousePos.y - m_oldMousePos.y;

	// rotate about the upVector
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-delta_mouse_X / look_speed_divisor), m_upVector);

	// rotate up/down relative to the rightVector
	glm::vec3 rightVector = glm::normalize(glm::cross(m_viewDirection, m_upVector));
	glm::mat4 rotationMatrix2 = glm::rotate(glm::mat4(1.0f), glm::radians(-delta_mouse_Y / look_speed_divisor), rightVector);

	// combine rotations
	glm::vec4 newDirection = rotationMatrix2 * rotationMatrix * glm::vec4(m_viewDirection, 0.0f);

	// update view direction
	m_viewDirection = glm::vec3(newDirection);

	// update old position
	m_oldMousePos = newMousePos;
}

void Camera::MoveForward(float speed) {
	m_eyePos.x += m_viewDirection.x * speed;
	m_eyePos.y += m_viewDirection.y * speed;
	m_eyePos.z += m_viewDirection.z * speed;
}

void Camera::MoveBackward(float speed) {
	m_eyePos.x -= m_viewDirection.x * speed;
	m_eyePos.y -= m_viewDirection.y * speed;
	m_eyePos.z -= m_viewDirection.z * speed;
}

void Camera::MoveLeft(float speed) {
	glm::vec3 rightVector = glm::normalize(glm::cross(m_viewDirection, m_upVector));

	m_eyePos.x -= speed / move_speed_divisor * rightVector.x;
	m_eyePos.z -= speed / move_speed_divisor * rightVector.z;
}

void Camera::MoveRight(float speed) {
	glm::vec3 rightVector = glm::normalize(glm::cross(m_viewDirection, m_upVector));

	m_eyePos.x += speed / move_speed_divisor * rightVector.x;
	m_eyePos.z += speed / move_speed_divisor * rightVector.z;
}

void Camera::MoveUp(float speed) {
	m_eyePos.y += speed;
}

void Camera::MoveDown(float speed) {
	m_eyePos.y -= speed;
}

void Camera::SetCameraEyePos(float x, float y, float z) {
	m_eyePos.x = x;
	m_eyePos.y = y;
	m_eyePos.z = z;
}

float Camera::GetEyeXPos() {
	return m_eyePos.x;
}

float Camera::GetEyeYPos() {
	return m_eyePos.y;
}

float Camera::GetEyeZPos() {
	return m_eyePos.z;
}

float Camera::GetViewXDir() {
	return m_viewDirection.x;
}

float Camera::GetViewYDir() {
	return m_viewDirection.y;
}

float Camera::GetViewZDir() {
	return m_viewDirection.z;
}

glm::vec3 Camera::GetCameraPos() {
	return m_eyePos;
}
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
public:
	Camera();

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	void MouseLook(int mouseX, int mouseY);
	void MoveForward(float speed);
	void MoveBackward(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);
	void MoveUp(float speed);
	void MoveDown(float speed);

	void SetCameraEyePos(float x, float y, float z);
	
	float GetEyeXPos();
	float GetEyeYPos();
	float GetEyeZPos();

	float GetViewXDir();
	float GetViewYDir();
	float GetViewZDir();

	glm::vec3 m_eyePos; // made public (not private)
	glm::vec3 GetCameraPos(); // not useful

private:
	glm::vec2 m_oldMousePos;
	glm::vec3 m_viewDirection;
	glm::vec3 m_upVector;

};


#endif
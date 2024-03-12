#ifndef CAMERA_H
#define CAMERA_H
#include "glm/glm.hpp"

class Camera 
{
public: 

	glm::mat4 getViewMatrix() const;
	virtual void rotate(float yaw, float pitch) {}

protected:
	Camera();

	glm::vec3 m_position;
	glm::vec3 m_targetPos;
	glm::vec3 m_up;

	// Eulers angles in radians
	float m_yaw;
	float m_pitch;

};


class OrbitCamera:public Camera
{
public: 
	OrbitCamera();

	virtual void rotate(float yaw, float pitch); // in degrees

	void setLookAt(const glm::vec3& target);
	void setRadius(float radius);

private: 

	void updateCameraVectors();
	float m_radius;
};




















#endif // CAMERA_H

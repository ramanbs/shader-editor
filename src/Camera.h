#ifndef CAMERA_H
#define CAMERA_H
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

class Camera 
{
public: 

	glm::mat4 getViewMatrix() const;
	virtual void rotate(float yaw, float pitch) {}
	virtual void setPosition(const glm::vec3 position) {}
	virtual void move(const glm::vec3& offsetPos) {}

	const glm::vec3& getLook() const;
	const glm::vec3& getRight() const;
	const glm::vec3& getUp() const;
	const glm::vec3& getPosition() const;

	float getFOV() const { return m_FOV; }
	void setFOV(float fov) { m_FOV = fov; }

protected:
	Camera();

	glm::vec3 m_position;
	glm::vec3 m_targetPos;
	glm::vec3 m_up;
	glm::vec3 m_look;
	glm::vec3 m_right;

	const glm::vec3 WORLD_UP;

	// Eulers angles in radians
	float m_yaw;
	float m_pitch;

	float m_FOV;

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

class FPSCamera : public Camera 
{
public: 

	FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0.0f);

	virtual void setPosition(const glm::vec3& position);
	virtual void rotate(float yaw, float pitch); // in degrees
	virtual void move(const glm::vec3& offsetPos);

private: 
	
	void updateCameraVectors();
};


#endif // CAMERA_H

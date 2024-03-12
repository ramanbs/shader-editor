#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera() 
	:m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
	 m_targetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
	 m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
	 m_yaw(0.0f),
	 m_pitch(0.0f)
{
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_position, m_targetPos, m_up);
}



OrbitCamera::OrbitCamera() 
	:m_radius(10.0f)
{

}

void OrbitCamera::setLookAt(const glm::vec3& target)
{
	m_targetPos = target;
}

void OrbitCamera::setRadius(float radius) 
{
	m_radius = glm::clamp(radius, 2.0f, 80.0f);
}

void OrbitCamera::rotate(float yaw, float pitch) 
{
	m_yaw = glm::radians(yaw);
	m_pitch = glm::radians(pitch);

	
	m_pitch = glm::clamp(m_pitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	updateCameraVectors();
}

void OrbitCamera::updateCameraVectors() 
{
	// Spherical to cartersian conversion - wiki Spherical Coordinate system
	// x = rsin(phi)sin(theta), y = rsin(phi), z = rcos(phi)cos(theta)
	// Yaw angle = theta (angle between z axis and camera y)
	// Pitch angle = phi (angle between camera target and pos to camera z )

	m_position.x = m_targetPos.x + m_radius * cosf(m_pitch) * sinf(m_yaw);
	m_position.y = m_targetPos.y + m_radius * sinf(m_pitch);
	m_position.z = m_targetPos.z + m_radius * cosf(m_pitch) * cosf(m_yaw);
}
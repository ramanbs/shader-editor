#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

const float DEF_FOV = 45.0f;

Camera::Camera() 
	:m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
	 m_targetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
	 m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
	 m_right(glm::vec3(0.0f, 0.0f, 0.0f)),
	 WORLD_UP(glm::vec3(0.0f, 1.0f, 0.0f)),
	 m_yaw(0.0f),
	 m_pitch(0.0f),
	 m_FOV(DEF_FOV)
{

}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_position, m_targetPos, m_up);
}

const glm::vec3& Camera::getLook() const
{
	return m_look;
}

const glm::vec3& Camera::getRight() const
{
	return m_right;
}

const glm::vec3& Camera::getUp() const
{
	return m_up;
}

const glm::vec3& Camera::getPosition() const
{
	return m_position;
}


// Orbit Camera

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

// First Person Camera

FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch)
{
	m_position = position;
	m_yaw = yaw;
	m_pitch = pitch;
}

void FPSCamera::setPosition(const glm::vec3& position)
{
	m_position = position;
}

void FPSCamera::move(const glm::vec3& offsetPos)
{
	m_position += offsetPos;
	updateCameraVectors();
}

void FPSCamera::rotate(float yaw, float pitch)
{
	m_yaw += glm::radians(yaw);
	m_pitch += glm::radians(pitch);

	m_pitch = glm::clamp(m_pitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	updateCameraVectors();
}

void FPSCamera::updateCameraVectors()
{
	glm::vec3 look;

	look.x = cosf(m_pitch) * sinf(m_yaw);
	look.y = sinf(m_pitch);
	look.z = cosf(m_pitch) * cosf(m_yaw);

	m_look = glm::normalize(look);

	m_right = glm::normalize(glm::cross(m_look, WORLD_UP));
	m_up = glm::normalize(glm::cross(m_right, m_look));

	m_targetPos = m_position + m_look;
}




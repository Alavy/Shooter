#include"Camera.h"
Camera::Camera(glm::vec3 pos, glm::vec3 cameraFront, glm::vec3 cameraUp, GLfloat fov, GLfloat aspectRatio,
	GLfloat nearPlane, GLfloat farPlane)
{
	m_aspectRatio = aspectRatio;

	m_viewMat = glm::lookAt(pos, pos + cameraFront, cameraUp);
	m_projectionMat = glm::perspective(fov,m_aspectRatio,nearPlane,farPlane);
}

void Camera::UpdateCameraMat(glm::vec3 pos, glm::vec3 cameraFront, glm::vec3 cameraUp)
{
	m_position = pos;
	m_viewMat = glm::lookAt(pos, pos + cameraFront, cameraUp);

}

glm::mat4 Camera::ReturnCameraMat()
{
	return m_projectionMat * m_viewMat;
}
glm::mat4 Camera::ReturnViewMat()
{
	return  m_viewMat;
}
glm::mat4 Camera::ReturnPerpectiveMat()
{
	return  m_projectionMat;
}

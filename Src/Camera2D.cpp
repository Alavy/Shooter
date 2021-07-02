#include"Camera2D.h"

Camera2D::Camera2D(glm::vec3 pos,int width,int height,
	float nearPlane, float farPlane)
{
	m_position = pos;
	m_width = width;
	m_height = height;

	m_viewMat = glm::lookAt(m_position, m_position + m_cameraDir, m_up);
	m_projectionMat = glm::ortho((float)-m_width/2,(float)
		m_width/2,(float)-m_height/2, (float)m_height/2);
}

void Camera2D::UpdateCameraMat(glm::vec3 pos)
{
	m_position = pos;
	m_viewMat = glm::lookAt(m_position, m_position + m_cameraDir, m_up);

}

glm::mat4 Camera2D::ReturnCameraMat()
{
	return m_projectionMat*m_viewMat;
}

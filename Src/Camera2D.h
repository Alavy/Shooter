#pragma once
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include"ShaderProgram.h"

class Camera2D
{
public:
	Camera2D(glm::vec3 pos, int width, int height,
		float nearPlane, float farPlane);
	void UpdateCameraMat(glm::vec3 pos);
	glm::mat4 ReturnCameraMat();
private:
	glm::vec3 m_position=glm::vec3(0.0f);
	glm::vec3 m_cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 m_projectionMat=glm::mat4(1.0f);
	glm::mat4 m_viewMat = glm::mat4(1.0f);

	int m_width = 0, m_height = 0;
};
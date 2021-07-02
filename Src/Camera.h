#pragma once
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include"ShaderProgram.h"
class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 cameraFront, glm::vec3 cameraUp,GLfloat fov ,GLfloat aspectRatio,
		GLfloat nearPlane, GLfloat farPlane);
	void UpdateCameraMat(glm::vec3 pos, glm::vec3 cameraFront, glm::vec3 cameraUp);
	glm::mat4 ReturnCameraMat();
	glm::mat4 ReturnViewMat();
	glm::mat4 ReturnPerpectiveMat();
	glm::vec3 Position() { return m_position; }
private:


	glm::mat4 m_projectionMat = glm::mat4(1.0f);
	glm::mat4 m_viewMat = glm::mat4(1.0f);
	glm::vec3 m_position = glm::vec3(0.0f);
	GLfloat m_aspectRatio = 0.0f;
};

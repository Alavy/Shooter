#pragma once
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include<vector>
#include<iostream>
#include<string>

#include"stb_image.h"
#include"ShaderProgram.h"
#include"VAO.h"
#include"VBO.h"
#include"Camera.h"

class SkyBox
{
public:
	SkyBox(Camera * camera);
	void Draw(float elapsedTime, float deltaTime);
	void Move(glm::vec3 pos, float angleDegree, glm::vec3 rotAxis = glm::vec3(0, 1, 0));

	void CleanUp();
private:
	GLuint m_cubeMapTextureID = -1;
	ShaderProgram* m_shaderProgram = nullptr;
	VAO* m_vao = nullptr;
	VBO* m_vbo = nullptr;
	Camera* m_camera = nullptr;

	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_RotAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	float m_angle = 0;

	glm::vec3 m_sizeInWorldSpace = glm::vec3(1.0f);

	GLuint loadCubemap(std::vector<std::string> faces);
};
#pragma once
#include<glad/glad.h>

#include"VAO.h"
#include"VBO.h"
#include "EBO.h"
#include"Texture2D.h"

#include"ShaderProgram.h"
#include"Camera2D.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<GLFW/glfw3.h>
#include<box2d/box2d.h>

class SpriteRenderer
{
public:
	SpriteRenderer(const char* textureFilePath,
		const char* vertexShaderFilePath,
		const char* fragShderFilePath,
		glm::vec3 pos,
		glm::vec3 size,
		Camera2D* camera2d, 
		glm::vec2 frameSize,
		b2World * world);
	~SpriteRenderer();
	void Draw(float elapsedTime, float deltaTime);
	void CleanUp();
	void Move(glm::vec3 pos, float angleDegree);
	void Scale(glm::vec3 scale);
	void Rotate(float angleDegree);
	void SetCurrentAnim(glm::uvec3 animData);
	GLfloat GetCurrentAnimIndex() { return m_currentAnimIndex; }
	void SetCurrentAnimIndex(GLfloat AnimIndex) { m_currentAnimIndex= AnimIndex; }

	void FreezeAnim() { m_freezeAnimMul = 0; }
	void ReverseAnim() { m_freezeAnimMul = -1; }
	void UnFreezeAnim() { m_freezeAnimMul = 1; }

	glm::vec3 Position() { return m_position; }
	glm::vec3 Size() { return m_sizeInWorldSpace; }

private:
	VAO* m_vao= nullptr;
	VBO* m_vbo_vert = nullptr;
	VBO* m_vbo_uv = nullptr;
	EBO* m_ebo = nullptr;
	Texture2D* m_texture = nullptr;

	ShaderProgram* m_shaderProgram= nullptr;
	Camera2D* m_camera2d = nullptr;


	GLuint m_elementCount = 6;
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_sizeInWorldSpace = glm::vec3(0.0f);

	GLfloat m_animStepX = 0;
	GLfloat m_animStepY = 0;
	GLfloat m_animSpeed = 60.0f;
	// row , number of frame
	glm::uvec3 m_currentAnim = glm::uvec3(0,0,0);

	GLfloat m_currentAnimIndex = 0;
	GLfloat m_freezeAnimMul = 1.0f;
	void animate(float elapsedTime, float deltaTime);
};

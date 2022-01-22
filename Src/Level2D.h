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
#include<vector>
#include<string>


class Level2D
{
public:
	Level2D(glm::vec3 pos, 
		glm::vec3 sizeInWorldSpace,
		Camera2D* camera,
		glm::vec2 frameSize,
		b2World* world);
	void Draw();
	void CleanUp();
	void Move(glm::vec3 pos, float angleDegree);
	glm::vec3 PlayerPosition() { return m_playerPos;  }
	std::vector<glm::vec3> EnemyPositions() { return m_enemyPos; }
	bool IsInWalkableTile(glm::vec3 pos);
	glm::vec3 FindNeighbourTile(glm::vec3 pos, GLfloat F, glm::vec3 targetPos);

	~Level2D();
private:
	VAO* m_vao = nullptr;
	VBO* m_vbo_vert = nullptr;
	Texture2D* m_texture = nullptr;
	GLuint m_howmanyVertices = 0;

	ShaderProgram* m_shaderProgram = nullptr;
	Camera2D* m_camera2d = nullptr;
	b2Body* m_body = nullptr;

	GLfloat m_uvStepX;
	GLfloat m_uvStepY;

	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_sizeInWorldSpace = glm::vec3(0.0f);
	glm::vec3 m_playerPos= glm::vec3(0.0f);
	std::vector<glm::vec3> m_enemyPos;
	std::vector<std::string> m_pathData;

	// methods
	void readLevelData(std::vector<std::string>& leveldata,const char* filePath);
	void calculateLevelVertices(const char* filePath, std::vector<GLfloat>& vertices,std::vector<b2Vec2>& collisionEdges);
	void insertVertices(std::vector<GLfloat>& vertices,int i,int j, GLfloat uvBaseX, GLfloat uvBaseY);
	void insertCollisionGeometry(std::vector<b2Vec2>& collisionEdges, int i, int j, bool firstVert);
	void calculateNavPath(const char* filePath);
};
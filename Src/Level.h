#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<bullet/btBulletCollisionCommon.h>
#include<bullet/btBulletDynamicsCommon.h>

#include<vector>

#include"Camera.h"
#include"VAO.h"
#include"VBO.h"
#include "EBO.h"
#include"Vertex.h"
#include"Texture2D.h"
#include"MeshRenderer.h"

class Level
{
public:
	Level(glm::vec3 pos,
		glm::vec3 sizeInWorldSpace,
		Camera* camera,
		glm::vec3 worldOffsetStep,
		btDiscreteDynamicsWorld* world);
	void Move(glm::vec3 pos, float angleDegree);
	void Update(float elapsedTime,float deltaTime);
	void Draw(float elapsedTime, float deltaTime);
	void CleanUp();

private:
	VAO* m_groundVao = nullptr;
	VBO* m_groundVbo_vert = nullptr;
	EBO* m_groundEbo = nullptr;
	GLuint m_howmanyIndicesForGround = 0;

	VAO* m_wallVao = nullptr;
	VBO* m_wallVbo_vert = nullptr;
	EBO* m_wallEbo = nullptr;
	GLuint m_howmanyIndicesForWall = 0;

	Texture2D* m_groundTexture = nullptr;
	ShaderProgram* m_grounShaderProgram = nullptr;

	Texture2D* m_wallTexture = nullptr;
	ShaderProgram* m_wallShaderProgram = nullptr;

	Camera* m_camera = nullptr;

	// objects
	std::vector<MeshRenderer*> m_meshOjects;
	

	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_sizeInWorldSpace = glm::vec3(1.0f);
	glm::vec3 m_worldOffsetStep = glm::vec3(1.0f);
	glm::vec3 m_lengthInWorldSpace = glm::vec3(1.0f);

	// for physics
	btCompoundShape* m_compundShape = nullptr;
	btScalar m_Mass = btScalar(0.f);
	btVector3 m_LocalInertia = btVector3(0, 0, 0);
	btRigidBody* m_rigidBoby = nullptr;
	GLfloat m_heightOftheWall = 4;


	void calculateLevelVertices(const char* filePath, std::vector<Vertex>& vertices, std::vector<Vertex>& wallVertices);
	void readLevelData(const char* filePath, std::vector<std::string>& leveldata);
	void insertVertices(std::vector<Vertex>& vertices, std::vector<Vertex>& wallVertices,int x, int z, GLfloat u, GLfloat v);
	void calculateStaticCollisionObjects(btDiscreteDynamicsWorld* world,GLfloat heightOftheWall);
};
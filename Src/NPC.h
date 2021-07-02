#pragma once
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<bullet/btBulletCollisionCommon.h>
#include<bullet/btBulletDynamicsCommon.h>

#include<iostream>
#include<vector>
#include <cmath>

#include"SkinMeshRenderer.h"

class NPC
{
public:
	NPC(const char* filePath, 
		glm::vec3 pos, 
		glm::vec3 size, 
		Camera* camera,
		btDiscreteDynamicsWorld* world);
	void Update(float elapsedTime, float deltaTime);
	void Draw(float elapsedTime, float deltaTime);
	void  ApplyForce(glm::vec3 forceDir);

	glm::vec3 Position() { return m_position; }
	void DecreaseHeath(GLfloat val) {
		m_health= m_health-val;
	}
	GLfloat CurrentHeath() {
		return m_health;
	}
	void CleanUp();

private:
	SkinMeshRenderer* m_meshRenderer = nullptr;
	glm::vec3 m_sizeInWorld = glm::vec3(1.0f);
	glm::vec3 m_position = glm::vec3(1.0f);

	glm::vec3 m_npcTargetDir = glm::vec3(0,0,-1);
	glm::vec3 m_npcCurrentDir = glm::vec3(0, 0, -1);
	float m_rotTargetAngle = 0;
	float m_rotCurrentAngle = 0;


	glm::vec3 m_npcRotAxis = glm::vec3(0, 1, 0);
	GLfloat m_previousTime = 0;

	GLfloat m_height = 1.0f;
	GLfloat m_health = 100;
	GLfloat m_Yoffset = .6;
	// physics Stuffs
	// physics
	btCollisionShape* m_collisionShape = nullptr;
	btScalar m_Mass = btScalar(1.f);
	btVector3 m_LocalInertia = btVector3(0, 0, 0);
	btDefaultMotionState* m_motionState = nullptr;
	btRigidBody* m_rigidBoby = nullptr;
	btDiscreteDynamicsWorld* m_world = nullptr;
	// methods
};
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

#include"MeshRenderer.h"

class Player
{
public:
	enum class AnimationState
	{
		IDLE,
		RUN,
		AIM
	};

	Player(const char* filePath,
		glm::vec3 cameraPos,
		glm::vec3 playerPos, 
		glm::vec3 size, Camera* camera,
		btDiscreteDynamicsWorld* world);
	void Update(float elapsedTime, float deltaTime, glm::vec3 cameraDir, glm::vec3 cameraUp);
	void Draw(float elapsedTime, float deltaTime);
	void ApplyForce(glm::vec3 forceDir);
	void SetVelocity(glm::vec3 velocityDir);
	AnimationState CurrentAnimState() { return m_currentAnimState; }
	void SetCurrentAnimState(AnimationState animState) { 
		m_previousAnimState = m_currentAnimState;
		m_currentAnimState = animState; 
	}
	bool IsPlayerOnGround() {
		if (abs(m_rigidBoby->getLinearVelocity().getY()) > 0.2) {
			return false;
		}
		return true;
	}
	void SetAim() {
		m_isInAim = !m_isInAim;
	}
	void Shoot() {
		m_isInShooting = true;
	}
	void HoldShoot() {
		m_isInShooting = false;
	}
	int NumberOfKill() {
		return m_numberOfKill;
	}
	int NumberOfAmmo() {
		return m_Ammo;
	}
	void DEcreaseNumberOFAMMO() {
		m_Ammo--;
		if (m_Ammo < 0) {
			m_Ammo = 0;
		}
	}
	void IncreaseNumberOFKill() {
		m_numberOfKill++;
	}
	void DebugSetPlayerWeaponPos(glm::vec3 pos);
	void CleanUp();
	
private:
	MeshRenderer* m_meshRenderer = nullptr;
	Camera* m_camera = nullptr;

	// game state
	int m_numberOfKill = 0;
	int m_Ammo = 1000;

	// anim state
	AnimationState m_currentAnimState = AnimationState::IDLE;
	AnimationState m_previousAnimState = AnimationState::IDLE;
	bool m_isInAim = false;
	bool m_isInShooting = false;

	std::vector<glm::vec3> idleAnim = {
		glm::vec3(0.114221, -0.172574, -0.217691),
		glm::vec3(0.124221, -0.182574, -0.227691)
	};
	float idleAnimDuration = 2.0f;
	float idleAnimSpeed = 3.0f;

	std::vector<glm::vec3> runAnim = {
		glm::vec3(0.114221, -0.172574, -0.217691),
		glm::vec3(0.0291477,  -0.180845, -0.179271)
	};
	float runAnimDuration = 0.8f;
	float runAnimSpeed = 10.0f;

	std::vector<glm::vec3> aimAnim = {
		glm::vec3(0.114221, -0.172574, -0.217691),
		glm::vec3(0.00233879, -0.202478, -0.158973)
	};
	float aimAnimDuration = 0.8f;
	float aimAnimSpeed = 10.0f;

	std::vector<glm::vec3> idleShootAnim = {
		glm::vec3(0.114221, -0.172574, -0.217691),
		glm::vec3(0.114221, -0.169188, -0.181168)
	};
	float idleShootAnimDuration = 0.3f;
	float idleShootAnimSpeed = 20.0f;

	std::vector<glm::vec3> aimShootAnim = {
		glm::vec3(0.00233879, -0.202478, -0.158973),
		glm::vec3(0.00233879,  -0.202478,  -0.10217)
	};
	float aimShootAnimDuration = .4f;
	float aimShootAnimSpeed = 17.0f;

	glm::vec3 m_cameraPos = glm::vec3(3.0f, 1.3f, 2);

	// physics
	btCollisionShape* m_collisionShape = nullptr;
	btScalar m_Mass = btScalar(1.f);
	btVector3 m_LocalInertia = btVector3(0, 0, 0);
	btDefaultMotionState* m_motionState = nullptr;
	btRigidBody* m_rigidBoby = nullptr;


	// methods
	void handleAnimation(float elapsedTime, float deltaTime);

	void idleAnimate(float elapsedTime , float deltaTime);
	void aimAnimate(float elapsedTime, float deltaTime);
	void runAnimate(float elapsedTime, float deltaTime);
	void idleShootAnimate(float elapsedTime, float deltaTime);
	void aimShootAnimate(float elapsedTime, float deltaTime);


};

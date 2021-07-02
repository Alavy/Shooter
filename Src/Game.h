#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include<bullet/btBulletCollisionCommon.h>
#include<bullet/btBulletDynamicsCommon.h>
#include"Camera.h"
#include"NPC.h"
#include"Player.h"
#include"Level.h"
#include"SkyBox.h"
#include"TextRenderer.h"
#include"BezierObject.h"
class Game
{
public:
	Game(int width, int height, GLFWwindow* window);
	void Init();
	void Update(float elapsedTime,float deltaTime);
	void Render(float elapsedTime, float deltaTime);
	void ProcessInput(float elapsedTime, float deltaTime);
	void CleanUp();
	void mouse_callback(double xpos, double ypos);

private:
	int m_width = 800, m_height = 500;
	GLFWwindow* m_window = nullptr;
	Camera* m_camera = nullptr;

	std::vector<NPC*> m_npcs;

	Player* m_player = nullptr;
	Level* m_level = nullptr;
	SkyBox* m_skyBox = nullptr;
	TextRenderer* m_textRenderer = nullptr;
	BezierObect* m_bezierObject = nullptr;
	// button state
	float m_previousTime = 0;
	GLuint m_InitNpcCount = 0;

	//for Physics Engine
	
	btDefaultCollisionConfiguration* m_CollisionConfig = nullptr;
	btCollisionDispatcher* m_CollisionDispatcher = nullptr;
	btBroadphaseInterface* m_OverlappingPairCache = nullptr;
	btSequentialImpulseConstraintSolver* m_Solver = nullptr;
	btDiscreteDynamicsWorld* m_DynamicsWorld = nullptr;

	GLuint m_maxSubStep = 10;

	//----> for fps player and camera
	glm::vec3 m_cameraPos = glm::vec3(7,1.3f,8);
	glm::vec3 m_cameraDir = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_payerPos = glm::vec3(0.114221, -0.172574, -0.217691);

	float m_lastMouseX = 0, m_lastMouseY = 0;
	float m_pitchAngle=0, m_yawAngle=0;
	bool m_firstTimeMouse = true;
	bool m_cursorIsActive = true;
	//////<----------

};

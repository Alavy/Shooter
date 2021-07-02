#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"SpriteRenderer.h"
#include"Camera2D.h"
#include"Level2D.h"
#include"Player2D.h"
#include"Enemy2D.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<box2d/box2d.h>

class Game2D
{
public:
	Game2D(int width, int height, GLFWwindow* window);
	void Init();
	void Update(float elapsedTime, float deltaTime);
	void Render(float elapsedTime, float deltaTime);
	void ProcessInput(float elapsedTime, float deltaTime);
	void CleanUp();
	void mouse_callback(double xpos, double ypos);


private:
	int m_width = 800, m_height = 500;
	GLFWwindow* m_window = nullptr;
	Level2D * m_level = nullptr;
	Player2D* m_player = nullptr;
	std::vector<Enemy2D*> m_enemys;
	b2World* m_world=nullptr;
	int32 m_velocityIterations = 6;
	int32 m_positionIterations = 2;

	Camera2D* m_camera2d = nullptr;
	glm::vec3 m_CameraPos = glm::vec3(0.0f);
	bool oneTime = false;

};
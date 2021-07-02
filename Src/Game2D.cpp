#include"Game2D.h"

Game2D::Game2D(int width,int height, GLFWwindow* window)
{
    m_height = height;
    m_width = width;
    m_window = window;
}
void Game2D::mouse_callback(double xpos, double ypos)
{
}


void Game2D::Init()
{
	// box2d init
	b2Vec2 gravity(0.0f, -10.0f);
	m_world = new b2World(gravity);

	m_CameraPos = glm::vec3(0, 0, 0);
	GLfloat aspectRatio = (GLfloat)m_width / m_height;
	m_camera2d = new Camera2D(m_CameraPos,
		10* aspectRatio,10, -1, 10);
	m_level = new Level2D(glm::vec3(0, 0, 0),
		glm::vec3(2, 2, 0), m_camera2d, glm::vec2(128, 128),m_world);

	m_player = new Player2D(m_world,
		m_level->PlayerPosition(),
		glm::vec3(2, 2, 0), m_camera2d,
		glm::vec2(600, 600));
	m_CameraPos = m_level->PlayerPosition();
	const std::vector<glm::vec3> enemyPos = m_level->EnemyPositions();
	for (int i = 0; i < enemyPos.size(); i++) {
		m_enemys.emplace_back(new Enemy2D(m_world,
			enemyPos[i],
			glm::vec3(2, 2, 0), m_camera2d,
			glm::vec2(300, 300)));
	}
}

void Game2D::Update(float elapsedTime, float deltaTime)
{
	m_world->Step(deltaTime, m_velocityIterations, m_positionIterations);

	m_camera2d->UpdateCameraMat(m_player->Position());
	m_player->Update(deltaTime);
	/// enemy update
	for (int i = 0; i < m_enemys.size(); i++) {
		m_enemys[i]->Update(deltaTime);
	}
}

void Game2D::Render(float elapsedTime, float deltaTime)
{
	m_player->Draw();
	m_level->Draw();

	for (int i = 0; i < m_enemys.size(); i++) {
		m_enemys[i]->Draw();
	}
}
void Game2D::CleanUp()
{
	m_player->CleanUp();
	m_level->CleanUp();
	for (int i = 0; i < m_enemys.size(); i++) {
		m_enemys[i]->CleanUp();
	}
	delete m_player;
	for (int i = 0; i < m_enemys.size(); i++) {
		delete m_enemys[i];
	}
	delete m_world;
	delete m_level;
	delete m_camera2d;

}
void Game2D::ProcessInput(float elapsedTime, float deltaTime)
{
	glm::vec2 playerForce = glm::vec2(0.0f);
	GLfloat ForceValue = 100.0f;
	GLfloat JumpValue = 1000.0f;

	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, true);

	if (glfwGetKey(m_window, GLFW_KEY_L) == GLFW_PRESS) {
		if (elapsedTime > m_previousTime + 1) {
			m_cursorIsActive = !m_cursorIsActive;
			m_previousTime = elapsedTime;
		}


	}
	if (m_cursorIsActive) {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	}
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		playerForce = playerForce + glm::vec2(-1,0);
		m_player->ApplyForce(glm::normalize(playerForce) * ForceValue);

	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		playerForce = playerForce + glm::vec2(1, 0);
		m_player->ApplyForce(glm::normalize(playerForce) * ForceValue);


	}
	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (m_player->IsOnGound()) {
			playerForce = playerForce + glm::vec2(0, 1);
			m_player->ApplyForce(glm::normalize(playerForce) * JumpValue);
		}
		

	}


}

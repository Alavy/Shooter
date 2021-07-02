#include"Game.h"

Game::Game(int width, int height, GLFWwindow* window)
{
	m_height = height;
	m_width = width;
	m_window = window;
}
void Game::mouse_callback(double xpos, double ypos)
{
	if (m_cursorIsActive) {
		if (m_firstTimeMouse)
		{
			m_lastMouseX = xpos;
			m_lastMouseY = ypos;
			m_firstTimeMouse = false;
		}
		float xoffset = xpos - m_lastMouseX;
		float yoffset = m_lastMouseY - ypos; // reversed: y ranges bottom to top
		m_lastMouseX = xpos;
		m_lastMouseY = ypos;
		const float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_yawAngle += xoffset;
		m_pitchAngle += yoffset;

		if (m_pitchAngle > 89.0f)
			m_pitchAngle = 89.0f;
		if (m_pitchAngle < -89.0f)
			m_pitchAngle = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_yawAngle)) * cos(glm::radians(m_pitchAngle));
		direction.y = sin(glm::radians(m_pitchAngle));
		direction.z = sin(glm::radians(m_yawAngle)) * cos(glm::radians(m_pitchAngle));
		m_cameraDir = glm::normalize(direction);

		//std::cout <<" Camera dir : " <<m_cameraDir.x << " " << m_cameraDir.y << " " << m_cameraDir.z << std::endl;

	}
}

void Game::Init()
{
	// physics implementation
	
	m_CollisionConfig = new btDefaultCollisionConfiguration();
	m_CollisionDispatcher = new btCollisionDispatcher(m_CollisionConfig);
	m_OverlappingPairCache = new btDbvtBroadphase();
	m_Solver = new btSequentialImpulseConstraintSolver();
	m_DynamicsWorld = new btDiscreteDynamicsWorld(m_CollisionDispatcher,
		m_OverlappingPairCache,
		m_Solver,
		m_CollisionConfig);
	m_DynamicsWorld->setGravity(btVector3(0, -10, 0));

	m_camera = new Camera(m_cameraPos,m_cameraDir,m_cameraUp,
		70, (GLfloat)m_width / m_height, 0.01, 100);

	// adding npcs 

	m_npcs.emplace_back(new NPC("Assets/3D/test/model.dae",
		glm::vec3(6, 2, 8),
		glm::vec3(.1, .1, .1),
		m_camera,
		m_DynamicsWorld));
	
	m_npcs.emplace_back(new NPC("Assets/3D/test/model.dae",
		glm::vec3(10, 2, 3),
		glm::vec3(.1, .1, .1),
		m_camera,
		m_DynamicsWorld));
	m_npcs.emplace_back(new NPC("Assets/3D/test/model.dae",
		glm::vec3(10, 2, 6),
		glm::vec3(.1, .1, .1),
		m_camera,
		m_DynamicsWorld));
	
	m_InitNpcCount = m_npcs.size();

	m_player = new Player("Assets/3D/weapon/weapon.obj",
		m_cameraPos,
		m_payerPos, glm::vec3(1, 1, 1),
		m_camera,
		m_DynamicsWorld);

	m_level = new Level(glm::vec3(0, 0, 0),
		glm::vec3(1, 1, 1), 
		m_camera,
		glm::vec3(1, 1, 1),
		m_DynamicsWorld);
	m_skyBox = new SkyBox(m_camera);
	m_bezierObject = new BezierObect(glm::vec3(5, 1, 6), glm::vec3(.1, .1, .1), m_camera, m_DynamicsWorld);

	m_textRenderer = new TextRenderer("Assets/fonts/Antonio-Bold.ttf",
		m_width, m_height);

}

void Game::Update(float elapsedTime, float deltaTime)
{
	m_player->Update(elapsedTime, deltaTime, m_cameraDir, m_cameraUp);
	//std::cout << " player pos : " << m_payerPos.x << " " << m_payerPos.y << " " << m_payerPos.z << std::endl;
	//m_player->DebugSetPlayerWeaponPos(m_payerPos);
	// erase npc
	std::vector<NPC*>::iterator it;
	for (int i = 0; i < m_npcs.size(); i++) {
		if (m_npcs[i]->CurrentHeath() <= 0) {
			it = m_npcs.begin() + i;
			m_npcs[i]->CleanUp();
			m_player->IncreaseNumberOFKill();
			m_npcs.erase(it);
		}
	}
	// updating the NPC 
	for (int i = 0; i < m_npcs.size(); i++) {
		m_npcs[i]->Update(elapsedTime, deltaTime);
	}
	m_bezierObject->Update(elapsedTime, deltaTime);
	m_level->Update(elapsedTime, deltaTime);
	m_DynamicsWorld->stepSimulation(deltaTime, m_maxSubStep);

}

void Game::Render(float elapsedTime, float deltaTime)
{
	m_player->Draw(elapsedTime, deltaTime);
	for (int i = 0; i < m_npcs.size(); i++) {
		m_npcs[i]->Draw(elapsedTime, deltaTime);
	}
	m_level->Draw(elapsedTime, deltaTime);
	m_bezierObject->Draw(elapsedTime, deltaTime);


	// sky box must drawn last
	m_skyBox->Draw(elapsedTime, deltaTime);

	//std::cout << m_player->NumberOfKill() << std::endl;
	std::stringstream ss;
	ss << m_player->NumberOfKill();
	std::string num;
	ss >> num;
	std::string kill = " KILL COUNT : " + num;
	std::string ammo;
	ss.clear();
	ss << m_player->NumberOfAmmo();
	ss >> ammo;
	std::string AMMO = " Ammo COUNT : " + ammo;


	m_textRenderer->RenderText(kill,
		25.0f, m_height - 55.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
	m_textRenderer->RenderText(AMMO,
		25.0f, m_height - 150.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
	if (m_player->NumberOfAmmo() <= 0) {
		m_textRenderer->RenderText(" YOU Lose Big Time",
			m_width/2, m_height/2, 1.0f, glm::vec3(1, 0.0, 0.0));
	}
	else if (m_player->NumberOfKill() >= m_InitNpcCount) {
		m_textRenderer->RenderText(" YOU Win Big Time",
			m_width / 2, m_height / 2, 1.0f, glm::vec3(.7, 0.6, 0.4));
	}
}
void Game::CleanUp()
{
	for (int i = 0; i < m_npcs.size(); i++) {
		m_npcs[i]->CleanUp();
	}
	m_player->CleanUp();
	m_level->CleanUp();
	m_skyBox->CleanUp();
	m_bezierObject->CleanUp();
	m_textRenderer->CleanUp();
	delete m_camera;
	for (int i = 0; i < m_npcs.size(); i++) {
		delete m_npcs[i];
	}
	delete m_player;
	delete m_level;
	delete m_skyBox;
	delete m_textRenderer;

}

void Game::ProcessInput(float elapsedTime, float deltaTime)
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, true);
	}
		
	if (glfwGetKey(m_window, GLFW_KEY_L) == GLFW_PRESS) {
		if (elapsedTime> m_previousTime+1) {
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

	//const float cameraSpeed = 4.25f*deltaTime; // adjust accordingly
	//float force = 20* deltaTime;
	float velocityValue = 90 * deltaTime;
	glm::vec3 dir = glm::normalize(glm::vec3(m_cameraDir.x,0,m_cameraDir.z));

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
		//m_cameraPos += cameraSpeed * m_cameraDir;
		//m_player->ApplyForce(force * dir);
		m_player->SetVelocity(velocityValue * dir);
	}
	
		
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
		//m_cameraPos -= cameraSpeed * m_cameraDir;
		//m_player->ApplyForce((-1)*force * dir);
		m_player->SetVelocity((-1)*velocityValue * dir);
	}
	
		
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
		//m_cameraPos -= glm::normalize(glm::cross(m_cameraDir, m_cameraUp)) * cameraSpeed;
		//m_player->ApplyForce((-1)*force *glm::normalize(glm::cross(dir, m_cameraUp)));
		m_player->SetVelocity((-1) * velocityValue * glm::normalize(glm::cross(dir, m_cameraUp)));
	}
	
		
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
		// m_cameraPos += glm::normalize(glm::cross(m_cameraDir, m_cameraUp)) * cameraSpeed;
		//m_player->ApplyForce( force* glm::normalize(glm::cross(dir, m_cameraUp)));
		m_player->SetVelocity(velocityValue * glm::normalize(glm::cross(dir, m_cameraUp)));
	}

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (elapsedTime > m_previousTime + 0.6) {
			m_player->SetAim();
			m_player->SetCurrentAnimState(Player::AnimationState::AIM);
			m_previousTime = elapsedTime;
		}
	}

	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		m_player->Shoot();

		if (elapsedTime > m_previousTime + 0.4) {

			glm::vec3 fromPos = m_camera->Position();
			glm::vec3 toPos = m_camera->Position() + 1000.0f * m_cameraDir;

			btVector3 from(fromPos.x, fromPos.y, fromPos.z);
			btVector3 to(toPos.x, toPos.y, toPos.z);

			btCollisionWorld::ClosestRayResultCallback closestResults(from, to);
			//closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

			m_DynamicsWorld->rayTest(from, to, closestResults);
			m_player->DEcreaseNumberOFAMMO();

			if (closestResults.hasHit())
			{
					/*
					std::cout << " hit point : " <<" " <<closestResults.m_hitPointWorld.getX()
						<<" " <<closestResults.m_hitPointWorld.getY() <<" "<<
						closestResults.m_hitPointWorld.getZ()<<std::endl;
					*/
				btVector3 result = closestResults.m_collisionObject->getWorldTransform().getOrigin();
					glm::vec3 hittWorldPos = glm::vec3(
						result.getX(),
						result.getY(),
						result.getZ()
					);

				for (int i = 0; i < m_npcs.size(); i++) {
					if (glm::distance2(m_npcs[i]->Position(),hittWorldPos)<0.001) {
						m_npcs[i]->DecreaseHeath(30);
						//std::cout << " health : " << m_npcs[i]->CurrentHeath() << std::endl;
						m_npcs[i]->ApplyForce(20.0f * m_cameraDir);
						break;
					}
				}
			}
		}
	}
	else {
		m_player->HoldShoot();
	}
	
	
	
	/*debug purpose
	float cameraSpeed = deltaTime*0.1f;
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
		m_payerPos -= glm::vec3(0, 1, 0) * cameraSpeed;
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
		m_payerPos += glm::vec3(0,1,0) * cameraSpeed;
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
		m_payerPos -= glm::vec3(1, 0, 0) * cameraSpeed;
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		m_payerPos += glm::vec3(1, 0, 0) * cameraSpeed;
	if (glfwGetKey(m_window,GLFW_KEY_N) == GLFW_PRESS)
		m_payerPos -= glm::vec3(0, 0, 1) * cameraSpeed;
	if (glfwGetKey(m_window, GLFW_KEY_M) == GLFW_PRESS)
		m_payerPos += glm::vec3(0, 0, 1) * cameraSpeed;
	*/

	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (m_player->IsPlayerOnGround() && elapsedTime> m_previousTime + 0.6f) {
			m_player->ApplyForce(500.0f * m_cameraUp);
			m_previousTime = elapsedTime;
		}
	}
}
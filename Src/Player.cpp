#include"Player.h"

Player::Player(const char* filePath,
	glm::vec3 cameraPos,
	glm::vec3 playerPos,
	glm::vec3 size,
	Camera* camera, 
	btDiscreteDynamicsWorld* world)
{
	m_camera = camera;
	m_cameraPos = cameraPos;
	m_meshRenderer = new MeshRenderer(filePath, playerPos,
		size, camera,MeshRenderer::RenderMode::STICKTOCAMERA);
	m_meshRenderer->Move(playerPos, -180, glm::vec3(0, 1, 0));

	m_collisionShape= new btCapsuleShape(btScalar(.5),btScalar(0.1));
	
	btTransform startTransform;
	startTransform.setIdentity();
	m_collisionShape->calculateLocalInertia(m_Mass, m_LocalInertia);
	startTransform.setOrigin(btVector3(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z));

	m_motionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_Mass, m_motionState,
		m_collisionShape, m_LocalInertia);

	m_rigidBoby = new btRigidBody(rbInfo);
	//m_rigidBoby->setFriction(btScalar(10.5f));
	m_rigidBoby->setAngularFactor(btVector3(0,1,0));
	m_rigidBoby->setDamping(.95, 0.9);
	world->addRigidBody(m_rigidBoby);
}

void Player::Update(float elapsedTime, float deltaTime, 
	glm::vec3 cameraDir, glm::vec3 cameraUp)
{
	btTransform trans;
	if (m_rigidBoby && m_rigidBoby->getMotionState())
	{
		m_rigidBoby->getMotionState()->getWorldTransform(trans);
	}
	glm::vec3 pos = glm::vec3(
		trans.getOrigin().getX(),
		trans.getOrigin().getY(), 
		trans.getOrigin().getZ());
	
	m_camera->UpdateCameraMat(pos, cameraDir, cameraUp);
	handleAnimation(elapsedTime, deltaTime);

}

void Player::handleAnimation(float elapsedTime, float deltaTime)
{
	btVector3 vel = m_rigidBoby->getLinearVelocity();
	//std::cout << vel.length() << std::endl;
	// for setting anim 
	if (vel.length() > 0.3) {
		SetCurrentAnimState(AnimationState::RUN);
	}
	else {
		if (!m_isInAim) {
			SetCurrentAnimState(AnimationState::IDLE);
		}
	}
	// 
	if (m_currentAnimState == AnimationState::IDLE) {
		if (m_isInShooting) {
			idleShootAnimate(elapsedTime, deltaTime);

		}
		else {
			idleAnimate(elapsedTime, deltaTime);
		}
	}
	else if (m_currentAnimState == AnimationState::RUN) {
		runAnimate(elapsedTime, deltaTime);
	}
	else if (m_currentAnimState == AnimationState::AIM) {
		if (m_isInShooting) {
			aimShootAnimate(elapsedTime, deltaTime);
		}
		else {
			aimAnimate(elapsedTime, deltaTime);
		}
	}
}

void Player::idleAnimate(float elapsedTime,float deltaTime)
{
	m_isInAim = false;
	m_isInShooting = false;

	elapsedTime = fmod(elapsedTime, idleAnimDuration);
	if (elapsedTime <= idleAnimDuration / 2) {
		m_meshRenderer->Move(glm::mix(m_meshRenderer->Position(),
			idleAnim[1], deltaTime*idleAnimSpeed), 0);
	}
	else if (elapsedTime > idleAnimDuration / 2) {
		m_meshRenderer->Move(glm::mix(m_meshRenderer->Position(),
			idleAnim[0], deltaTime * idleAnimSpeed), 0);
	}
}
void Player::aimAnimate(float elapsedTime, float deltaTime)
{
	if (m_isInAim) {
		m_meshRenderer->Move(glm::mix(m_meshRenderer->Position(),
			aimAnim[1], deltaTime * aimAnimSpeed), 0);
	}
	else{
		m_meshRenderer->Move(glm::mix(m_meshRenderer->Position(),
			aimAnim[0], deltaTime * aimAnimSpeed), 0);
	}
}
void Player::runAnimate(float elapsedTime, float deltaTime)
{
	m_isInAim = false;
	m_isInShooting = false;
	elapsedTime = fmod(elapsedTime, runAnimDuration);
	if (elapsedTime <= runAnimDuration / 2) {
		m_meshRenderer->Move(glm::mix(m_meshRenderer->Position(),
			runAnim[1], deltaTime*runAnimSpeed), 0);
	}
	else if (elapsedTime > runAnimDuration / 2) {
		m_meshRenderer->Move(glm::mix(m_meshRenderer->Position(),
			runAnim[0], deltaTime * runAnimSpeed), 0);
	}
}
void Player::idleShootAnimate(float elapsedTime, float deltaTime)
{
	elapsedTime = fmod(elapsedTime, idleShootAnimDuration);
	if (elapsedTime <= idleShootAnimDuration / 2) {
		m_meshRenderer->Move(glm::mix(m_meshRenderer->Position(),
			idleShootAnim[1], deltaTime * idleShootAnimSpeed), 0);
	}
	else if (elapsedTime > idleShootAnimDuration / 2) {
		m_meshRenderer->Move(glm::mix(m_meshRenderer->Position(),
			idleShootAnim[0], deltaTime * idleShootAnimSpeed), 0);
	}
}
void Player::aimShootAnimate(float elapsedTime, float deltaTime)
{
	elapsedTime = fmod(elapsedTime, aimShootAnimDuration);
	if (elapsedTime <= aimShootAnimDuration / 2) {
		m_meshRenderer->Move(glm::mix(m_meshRenderer->Position(),
			aimShootAnim[1], deltaTime * aimShootAnimSpeed), 0);
	}
	else if (elapsedTime > aimShootAnimDuration / 2) {
		m_meshRenderer->Move(glm::mix(m_meshRenderer->Position(),
			aimShootAnim[0], deltaTime * aimShootAnimSpeed), 0);
	}
}

void Player::Draw(float elapsedTime, float deltaTime)
{
	m_meshRenderer->Draw(elapsedTime,deltaTime);
}

void Player::ApplyForce(glm::vec3 forceDir)
{
	m_rigidBoby->activate(true);
	m_rigidBoby->applyForce(
		btVector3(forceDir.x, forceDir.y, forceDir.z),
		btVector3(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z));
}

void Player::SetVelocity(glm::vec3 velocityDir)
{
	if (IsPlayerOnGround()) {
		m_rigidBoby->activate(true);
		m_rigidBoby->setLinearVelocity(btVector3(velocityDir.x, velocityDir.y, velocityDir.z));
	}
	
}

void Player::DebugSetPlayerWeaponPos(glm::vec3 pos)
{
	m_meshRenderer->Move(pos, 0);
}

void Player::CleanUp()
{
	m_meshRenderer->CleanUp();
	delete m_meshRenderer;

	delete m_collisionShape;
	delete m_motionState;
	delete m_rigidBoby;
}



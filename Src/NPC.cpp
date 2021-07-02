#include"NPC.h"

#include"Player.h"

NPC::NPC(const char* filePath,
	glm::vec3 pos,
	glm::vec3 size, Camera* camera,
	btDiscreteDynamicsWorld* world)
{
	m_sizeInWorld = size;
	m_position = pos;
	m_world = world;

	m_meshRenderer = new SkinMeshRenderer(filePath, m_position,
		size, camera);
	m_meshRenderer->SetDefaultRot(180,glm::vec3(0,0,1));
	m_meshRenderer->Move(m_position, 0);
	m_height = 0.3f;
	
	m_collisionShape = new btCapsuleShape(btScalar(.2), btScalar(m_height));
	btTransform startTransform;
	startTransform.setIdentity();

	m_collisionShape->calculateLocalInertia(m_Mass, m_LocalInertia);
	startTransform.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
	

	m_motionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(m_Mass, m_motionState,
		m_collisionShape, m_LocalInertia);

	m_rigidBoby = new btRigidBody(rbInfo);
	//m_rigidBoby->setFriction(btScalar(10.5f));
	m_rigidBoby->setAngularFactor(btVector3(0, 1, 0));
	m_rigidBoby->setDamping(.6, 0.7);

	m_world->addRigidBody(m_rigidBoby);
}
void  NPC::ApplyForce(glm::vec3 forceDir)
{
	m_rigidBoby->activate(true);
	m_rigidBoby->applyForce(
		btVector3(forceDir.x, forceDir.y, forceDir.z),
		btVector3(m_position.x, m_position.y, m_position.z));
}

void NPC::Update(float elapsedTime, float deltaTime)
{

	//std::cout << " npc pos : " << pos.x << " " << pos.y << " "<< pos.z<<std::endl;
	// wandering 
	float steerSpeed = 1.0f;
	if (elapsedTime > m_previousTime +2) {
		m_npcTargetDir = glm::vec3(0, 0, -1);
		//m_npcCurrentDir = glm::vec3(0, 0, -1);
		//m_rotCurrentAngle = 0;

		m_rotTargetAngle = rand() % 360;
		//std::cout << angle << std::endl;
		m_npcTargetDir = glm::rotateY(m_npcTargetDir, -glm::radians(m_rotTargetAngle));
		m_previousTime = elapsedTime;
	}
	//m_npcCurrentDir = glm::mix(m_npcCurrentDir, m_npcTargetDir, deltaTime * steerSpeed);
	ApplyForce(5.0f * m_npcTargetDir);

	btTransform trans;
	if (m_rigidBoby && m_rigidBoby->getMotionState())
	{
		m_rigidBoby->getMotionState()->getWorldTransform(trans);
	}
	glm::vec3 pos = glm::vec3(
		trans.getOrigin().getX(),
		trans.getOrigin().getY(),
		trans.getOrigin().getZ());
	m_position = pos;
	pos = glm::vec3(
		trans.getOrigin().getX(),
		trans.getOrigin().getY() - m_Yoffset,
		trans.getOrigin().getZ());
	//m_rotCurrentAngle = glm::mix(m_rotCurrentAngle, m_rotTargetAngle, deltaTime * steerSpeed);
	m_meshRenderer->Move(pos, m_rotTargetAngle, m_npcRotAxis);

}

void NPC::Draw(float elapsedTime, float deltaTime)
{
	m_meshRenderer->Draw(elapsedTime, deltaTime);
}

void NPC::CleanUp()
{
	m_meshRenderer->CleanUp();
	m_world->removeRigidBody(m_rigidBoby);
	
	delete m_meshRenderer;
	delete m_collisionShape;
	delete m_motionState;
	delete m_rigidBoby;
}
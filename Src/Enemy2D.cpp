#include"Enemy2D.h"

Enemy2D::Enemy2D(b2World* world,
    glm::vec3 pos,
    glm::vec3 size,
    Camera2D* camera2d,
    Player2D* player,
    Level2D* level,
    glm::vec2 frameSize)
{
    m_sprite = new SpriteRenderer("Assets/2D/mummy.png",
        "sprite.vert", "sprite.frag",
        pos,
        size,
        camera2d,
        frameSize, world);
    m_player = player;
    m_level = level;
#pragma region physics_initialization
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    m_body = world->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(size.x / 2.0, size.y / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    m_body->CreateFixture(&fixtureDef);
#pragma endregion
}

void Enemy2D::Draw(float elapsedTime, float deltaTime)
{
    m_sprite->Draw(elapsedTime, deltaTime);
}

void Enemy2D::Update(float elapsedTime, float deltaTime)
{
    findPath();
    // get sprite velocity
    glm::vec2 velocity = glm::vec2(m_body->GetLinearVelocity().x,
        m_body->GetLinearVelocity().y);

    // handling step
    if (std::abs(velocity.x) > 0.1) {
        m_sprite->UnFreezeAnim();
        m_sprite->SetCurrentAnim(walkAnim);
    }
    else {
        m_sprite->UnFreezeAnim();
        m_sprite->SetCurrentAnim(idleAnim);
    }

    if (velocity.x > 0.1) {
        if (m_isFlip) {
            glm::vec3 sizeInWorldSpace = m_sprite->Size();
            m_sprite->Scale(glm::vec3(-sizeInWorldSpace.x, sizeInWorldSpace.y, 0));
            m_isFlip = false;
        }
    }
    else if (velocity.x < -0.1) {
        if (!m_isFlip) {
            m_isFlip = true;
            glm::vec3 sizeInWorldSpace = m_sprite->Size();
            m_sprite->Scale(glm::vec3(-sizeInWorldSpace.x, sizeInWorldSpace.y, 0));
        }

    }
    m_sprite->Move(glm::vec3(m_body->GetPosition().x, m_body->GetPosition().y, 0), 0.0f);
    //m_level->IsInWalkableTile(m_player->Position());
}

void Enemy2D::ApplyForce(glm::vec2 force)
{
    b2Vec2 forceVal = b2Vec2(force.x, force.y);
    m_body->ApplyForce(forceVal, m_body->GetWorldCenter(), true);
}

void Enemy2D::CleanUp()
{
    m_sprite->CleanUp();
}

bool Enemy2D::IsOnGound()
{
    if (std::abs(m_body->GetLinearVelocity().y) < 0.008f) {
        return true;
    }
    return false;
}

Enemy2D::~Enemy2D()
{

}

void Enemy2D::findPath()
{
    glm::vec3  pos = m_level->FindNeighbourTile(Position(), 0, m_player->Position());
    

    /*while (glm::distance(dir,m_player->Position())>2.5f) {
        dir = m_level->FindNeighbourTile(dir, 0, m_player->Position());
    }*/
    //ApplyForce(glm::normalize(m_player->Position() - Position()) * 16.0f);
    ApplyForce(glm::normalize(pos-Position()) * 16.0f);

}

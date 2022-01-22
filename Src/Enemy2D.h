#pragma once
#include"SpriteRenderer.h"
#include"Player2D.h"
#include"Level2D.h"
#include<box2d/box2d.h>

class Enemy2D
{
public:
	Enemy2D(b2World* world,
		glm::vec3 pos,
		glm::vec3 size,
		Camera2D* camera2d,
		Player2D* player,
		Level2D* level,
		glm::vec2 frameSize);
	void Draw(float elapsedTime, float deltaTime);
	void Update(float elapsedTime, float deltaTime);
	glm::vec3 Position() { return m_sprite->Position(); }
	void ApplyForce(glm::vec2 force);
	void CleanUp();
	bool IsOnGound();
	~Enemy2D();

private:
	SpriteRenderer* m_sprite = nullptr;
	b2Body* m_body = nullptr;
	Player2D* m_player = nullptr;
	Level2D* m_level = nullptr;
	bool m_isFlip = false;
	// column row speed
	glm::uvec3 idleAnim = glm::uvec3(0, 9, 4);
	glm::uvec3 deadAnim = glm::uvec3(1, 9, 4);
	glm::uvec3 attackAnim = glm::uvec3(2, 9, 4);
	glm::uvec3 walkAnim = glm::uvec3(3, 9, 4);
	void findPath();

};


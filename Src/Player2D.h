#pragma once
#include"SpriteRenderer.h"
#include<box2d/box2d.h>

class Player2D
{
public:
	Player2D(b2World* world,
		glm::vec3 pos,
		glm::vec3 size,
		Camera2D* camera2d,
		glm::vec2 frameSize);
	void Draw(float elapsedTime,float deltaTime);
	void Update(float elapsedTime, float deltaTime);
	glm::vec3 Position() { return m_sprite->Position(); }
	void ApplyForce(glm::vec2 force);
	void CleanUp();
	bool IsOnGound();
	~Player2D();

private:
	SpriteRenderer* m_sprite = nullptr;
	b2Body* m_body = nullptr;
	bool m_isFlip = false;


	// column row speed
	glm::uvec3 idleAnim = glm::uvec3(0, 9, 4);
	glm::uvec3 deadAnim = glm::uvec3(1, 9, 4);
	glm::uvec3 jumpAnim = glm::uvec3(2, 9, 3);
	glm::uvec3 attackAnim = glm::uvec3(3, 9, 4);
	glm::uvec3 runAnim = glm::uvec3(4, 9, 4);

};

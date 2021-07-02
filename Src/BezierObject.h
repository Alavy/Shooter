#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<bullet/btBulletCollisionCommon.h>
#include<bullet/btBulletDynamicsCommon.h>

#include<vector>

#include"Camera.h"
#include"VAO.h"
#include"VBO.h"
#include "EBO.h"
#include"Vertex.h"
#include"Texture2D.h"

class BezierObect
{
public:
	BezierObect(glm::vec3 pos,
		glm::vec3 sizeInWorldSpace,
		Camera* camera,
		btDiscreteDynamicsWorld* world);

	void Move(glm::vec3 pos, float angleDegree, glm::vec3 rotAxis);
	void Update(float elapsedTime, float deltaTime);
	void Draw(float elapsedTime, float deltaTime);
	void CleanUp();
private:
	const double PI = 3.14159265389;
	int L = 20;
	const int dgre = 3;
	int ncpt = L + 1;
	int clikd = 0;
	const int nt = 40;
	const int ntheta = 20;


	VAO* m_vao = nullptr;
	VBO* m_vbo = nullptr;
	GLuint m_howmanyVertices = 0;

	Texture2D* m_texture = nullptr;
	ShaderProgram* m_shaderProgram = nullptr;

    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_sizeInWorldSpace = glm::vec3(0.0f);
	glm::vec3 m_RotAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	float m_angle = 0;

	// physics
	btCollisionShape* m_collisionShape = nullptr;
	btScalar m_Mass = btScalar(1.f);
	btVector3 m_LocalInertia = btVector3(0, 0, 0);
	btDefaultMotionState* m_motionState = nullptr;
	btRigidBody* m_rigidBoby = nullptr;

	Camera* m_camera = nullptr;
    void calculateVertices(std::vector<GLfloat>& vertices);
    //control points
    long long nCr(int n, int r);

    //polynomial interpretation for N points
	void BezierCurve(double t, float xy[2], float ctrlpoints[][3]);
};
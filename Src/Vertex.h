#pragma once
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	// methods
	static glm::mat4 AssimpToGlmMatrix(aiMatrix4x4 mat);
	static glm::quat AssimpToGlmQuat(aiQuaternion quat);
	static glm::vec3 AssimpToGlmVec3(aiVector3D vec);
};



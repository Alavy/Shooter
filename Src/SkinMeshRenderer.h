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

#include<iostream>
#include<vector>
#include<unordered_map>

#include"ShaderProgram.h"
#include"SkinMesh.h"
#include"SkinMeshVertex.h"
#include"Texture.h"
#include"Camera.h"


class SkinMeshRenderer
{
public:
	SkinMeshRenderer(const char* filePath,glm::vec3 pos, glm::vec3 size,Camera * camera);
	void Draw(float elapsedTime, float deltaTime);
	void Move(glm::vec3 pos, float angleDegree, glm::vec3 rotAxis = glm::vec3(0, 1, 0));

	void Scale(glm::vec3 scale);
	void Rotate(float angleDegree, glm::vec3 rotAxis = glm::vec3(0, 1, 0));
	void SetDefaultRot(float angle, glm::vec3 rotAxis) {
		m_defaultAngle = angle;
		m_defaultRotAxis = rotAxis;
	}
	void CleanUp();

	struct Bone {
		int id = 0; // position of the bone in final upload array
		std::string name = "";
		glm::mat4 localTransForm = glm::mat4(1.0f);
		std::vector<Bone> children;
	};

	// sturction representing an animation track
	struct BoneTransformTrack {
		std::vector<float> positionTimestamps;
		std::vector<float> rotationTimestamps;
		std::vector<float> scaleTimestamps;

		std::vector<glm::vec3> positions;
		std::vector<glm::quat> rotations;
		std::vector<glm::vec3> scales;
	};

	// structure containing animation information
	struct Animation {
		float duration = 0.0f;
		float ticksPerSecond = 1.0f;
		std::unordered_map<std::string, BoneTransformTrack> boneTransforms;
	};
private:
	// model data
	std::vector<SkinMesh> m_meshes;
	std::vector<Texture> m_textures_loaded;
	std::string m_directory;

	ShaderProgram* m_shaderProgram =nullptr;
	glm::vec3 m_position=glm::vec3(0.0f);
	glm::vec3 m_RotAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	float m_angle = 0;
	glm::vec3 m_defaultRotAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	float m_defaultAngle = 0;

	glm::vec3 m_sizeInWorldSpace = glm::vec3(1.0f);
	Camera* m_camera = nullptr;


	GLuint m_boneCount = 0;
	std::unordered_map<std::string, std::pair<GLuint, glm::mat4>> m_boneInfo = {};
	Bone m_skeleton;
	Animation m_animation;
	glm::mat4 m_globalInverseTransform=glm::mat4(1.0f);
	std::vector<glm::mat4> m_currentPose = {};
	glm::mat4 m_parentTransform = glm::mat4(1.0f);
	bool m_hasAnimation=true;
	const aiScene* m_scene=nullptr;


	// methods
	void processNode(aiNode* node);
	SkinMesh processMesh(aiMesh* mesh);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName);

	void debugBone(Bone& bone);
	void readSkeleton(Bone& boneOutput, aiNode* node);
	void loadAnimation(Animation& animatio);
	std::pair<GLuint, float>  getTimeFraction(std::vector<float>& times, float dt);
	void getPose(Bone& skeletion, float elapsedTime,std::vector<glm::mat4>& output,glm::mat4& parentTransform);
	
};


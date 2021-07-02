#pragma once

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<GLFW/glfw3.h>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include<iostream>
#include<vector>
#include<unordered_map>

#include"ShaderProgram.h"
#include"Mesh.h"
#include"Texture.h"
#include"Camera.h"



class MeshRenderer
{
public:
	enum class RenderMode
	{
		STICKTOCAMERA,
		NORMAL
	};

	MeshRenderer(const char* filePath, glm::vec3 pos, glm::vec3 size, Camera* camera,RenderMode rendermode);
	void Draw(float elapsedTime, float deltaTime);
	void Move(glm::vec3 pos, float angleDegree, glm::vec3 rotAxis = glm::vec3(0, 1, 0));
	glm::vec3 Position() { return m_position; }
	void Scale(glm::vec3 scale);
	void Rotate(float angleDegree, glm::vec3 rotAxis = glm::vec3(0, 1, 0));
	void CleanUp();


	
private:
	// model data
	std::vector<Mesh> m_meshes;
	std::vector<Texture> m_textures_loaded;
	std::string m_directory;

	ShaderProgram* m_shaderProgram = nullptr;
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_RotAxis = glm::vec3(0.0f,1.0f,0.0f);
	float m_angle = 0;

	glm::vec3 m_sizeInWorldSpace = glm::vec3(1.0f);
	Camera* m_camera = nullptr;
	RenderMode m_mode = RenderMode::NORMAL;

	bool m_hasAnimation = true;
	const aiScene* m_scene = nullptr;
	glm::mat4 m_globalInverseTransform = glm::mat4(1.0f);



	// methods
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName);

};

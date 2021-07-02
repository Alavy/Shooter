#include"MeshRenderer.h"

MeshRenderer::MeshRenderer(const char* filePath, glm::vec3 pos,
	glm::vec3 size, Camera* camera,
	RenderMode mode)
{
	m_shaderProgram = new ShaderProgram("normalmesh.vert", "normalmesh.frag");
	m_position = pos;
	m_sizeInWorldSpace = size;
	m_camera = camera;
	m_mode = mode;
	Move(pos, 0);

	Assimp::Importer import;
	m_scene = import.ReadFile(filePath, aiProcess_Triangulate|| aiProcess_FlipUVs);
	if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!m_scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	m_hasAnimation = m_scene->HasAnimations();
	std::string filepath(filePath);
	m_directory = filepath.substr(0, filepath.find_last_of("/"));

	m_globalInverseTransform = glm::inverse(Vertex::AssimpToGlmMatrix(
		m_scene->mRootNode->mTransformation));

	// calculate mesh and it's vertices
	processNode(m_scene->mRootNode, m_scene);
}

void MeshRenderer::Draw(float elapsedTime, float deltaTime)
{
	m_shaderProgram->Use();
	if (m_mode == RenderMode::NORMAL) {
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID,
			"CameraMat"),
			1, GL_FALSE,
			glm::value_ptr(m_camera->ReturnCameraMat()));
	}
	else if (m_mode == RenderMode::STICKTOCAMERA) {
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID,
			"CameraMat"),
			1, GL_FALSE,
			glm::value_ptr(m_camera->ReturnPerpectiveMat()));
	}


	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].Draw(*m_shaderProgram);
	}
}
void MeshRenderer::CleanUp()
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].CleanUp();
	}
}

void MeshRenderer::Move(glm::vec3 pos, float angleDegree, glm::vec3 rotAxis)
{
	m_position = pos;
	glm::mat4 modelMat = glm::mat4(1);
	modelMat = glm::translate(modelMat, m_position);
	if (angleDegree != 0.0f) {
		m_RotAxis = rotAxis;
		m_angle = angleDegree;
		
	}
	modelMat = glm::rotate(modelMat,
		glm::radians(m_angle), m_RotAxis);

	modelMat = glm::scale(modelMat, m_sizeInWorldSpace);
	m_shaderProgram->Use();

	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID, "ModelMat"),
		1, GL_FALSE,
		glm::value_ptr(modelMat));
}

void MeshRenderer::Scale(glm::vec3 scale)
{
	m_sizeInWorldSpace = scale;
	glm::mat4 modelMat = glm::mat4(1);
	modelMat = glm::translate(modelMat, m_position);
	modelMat = glm::scale(modelMat, scale);
	m_shaderProgram->Use();

	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID, "ModelMat"),
		1, GL_FALSE,
		glm::value_ptr( modelMat));
}

void MeshRenderer::Rotate(float angleDegree, glm::vec3 rotAxis)
{
	glm::mat4 modelMat = glm::mat4(1);
	modelMat = glm::translate(modelMat, m_position);
	modelMat = glm::rotate(modelMat,
		glm::radians(angleDegree), rotAxis);
	m_shaderProgram->Use();

	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID, "ModelMat"),
		1, GL_FALSE,
		glm::value_ptr( modelMat));
}

void MeshRenderer::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node’s meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}
Mesh MeshRenderer::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// pos
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}
	// indices for mesh
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(),
			specularMaps.end());
	}
	return Mesh(vertices, indices, textures);
}
std::vector<Texture> MeshRenderer::loadMaterialTextures(aiMaterial* mat, aiTextureType type,
	std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// debug purpose
		//std::cout << str.C_Str() << std::endl;
		//std::cout << directory << std::endl;

		bool skip = false;
		for (unsigned int j = 0; j < m_textures_loaded.size(); j++)
		{
			if (std::strcmp(m_textures_loaded[j].Path.data(),
				str.C_Str()) == 0)
			{
				textures.push_back(m_textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{ // if texture hasn’t been loaded already, load it
			Texture texture;
			texture.ID = Texture::TextureFromFile(str.C_Str(), m_directory);
			texture.Type = typeName;
			texture.Path = str.C_Str();
			textures.push_back(texture);
			m_textures_loaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

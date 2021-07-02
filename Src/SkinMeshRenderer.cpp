#include"SkinMeshRenderer.h"

SkinMeshRenderer::SkinMeshRenderer(const char* filePath, glm::vec3 pos, glm::vec3 size, Camera* camera)
{
	m_shaderProgram = new ShaderProgram("skinmesh.vert", "skinmesh.frag");
	m_position = pos;
	m_sizeInWorldSpace = size;
	m_camera = camera;

	Assimp::Importer import;
	m_scene = import.ReadFile(filePath, aiProcess_Triangulate );
	if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!m_scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	m_hasAnimation = m_scene->HasAnimations();
	std::string filepath(filePath);
	m_directory = filepath.substr(0, filepath.find_last_of("/"));

	m_globalInverseTransform = glm::inverse(SkinMeshVertex::AssimpToGlmMatrix(
		m_scene->mRootNode->mTransformation));

	// calculate mesh and it's vertices
	processNode(m_scene->mRootNode);

	if (m_hasAnimation) {
		// calculate skeleton
		readSkeleton(m_skeleton, m_scene->mRootNode);
		// load Animations
		loadAnimation(m_animation);

		m_currentPose.resize(m_boneInfo.size(), glm::mat4(1.0f));
		
		/*
		std::cout << "-->yo yo start" << std::endl;
		debugBone(m_skeleton);
		std::cout << "<--yo yo end" << std::endl;
		for (std::unordered_map<std::string, std::pair<GLuint, glm::mat4>>::const_iterator it = m_boneInfo.begin();
			it != m_boneInfo.end(); ++it)
		{
			std::cout << it->first << " " << "\n";
		}
		std::cout << m_boneInfo.size() << std::endl;

		std::cout << " starting point-----> bone " << std::endl;
		for (std::unordered_map<std::string, BoneTransformTrack>::const_iterator it = m_animation.boneTransforms.begin();
			it != m_animation.boneTransforms.end(); ++it)
		{

			std::cout << it->first << " " << "\n";

			std::cout << it->second.positionTimestamps.size() << " " << "\n";
			std::cout << it->second.rotationTimestamps.size() << " " << "\n";
			std::cout << it->second.scaleTimestamps.size() << " " << "\n";
			std::cout << " -----> " << std::endl;
			std::cout << it->second.positions.size() << " " << "\n";
			std::cout << it->second.rotations.size() << " " << "\n";
			std::cout << it->second.scales.size() << " " << "\n";

		}
		std::cout << " ending point-----> bone " << std::endl;
		*/


	}
	// default start pos
	Move(m_position, 0);
}
void SkinMeshRenderer::debugBone(Bone& bone) {
	std::cout << bone.name << std::endl;

	for (Bone& child : bone.children)
	{
		debugBone(child);
	}
}
void SkinMeshRenderer::Draw(float elapsedTime, float deltaTime)
{
	m_shaderProgram->Use();


	if (m_hasAnimation) {
		getPose(m_skeleton, elapsedTime, m_currentPose,
			m_parentTransform);

		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID,
			"bone_transforms"),
			m_boneInfo.size(), GL_FALSE, glm::value_ptr(m_currentPose[0]));
	}
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID,
		"CameraMat"),
		1, GL_FALSE,
		glm::value_ptr(m_camera->ReturnCameraMat()));


	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].Draw(*m_shaderProgram);
	}
}
void SkinMeshRenderer::CleanUp()
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].CleanUp();
	}
}

void SkinMeshRenderer::Move(glm::vec3 pos, float angleDegree,glm::vec3 rotAxis)
{
	m_position = pos;
	glm::mat4 modelMat = glm::mat4(1);
	modelMat = glm::translate(modelMat, m_position);
	if (angleDegree != 0.0f) {
		m_RotAxis = rotAxis;
		m_angle = angleDegree;

	}
	modelMat = glm::rotate(modelMat,
		glm::radians(m_defaultAngle), m_defaultRotAxis);
	modelMat = glm::rotate(modelMat,
		glm::radians(m_angle), m_RotAxis);

	modelMat = glm::scale(modelMat, m_sizeInWorldSpace);
	m_shaderProgram->Use();

	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID, "ModelMat"),
		1, GL_FALSE,
		glm::value_ptr(modelMat));
}


void SkinMeshRenderer::Scale(glm::vec3 scale)
{
	m_sizeInWorldSpace = scale;
	glm::mat4 modelMat = glm::mat4(1);
	modelMat = glm::translate(modelMat, m_position);
	modelMat = glm::scale(modelMat, scale);
	m_shaderProgram->Use();

	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID, "ModelMat"),
		1, GL_FALSE,
		glm::value_ptr(modelMat));
}

void SkinMeshRenderer::Rotate(float angleDegree, glm::vec3 rotAxis)
{
	glm::mat4 modelMat = glm::mat4(1);
	modelMat = glm::translate(modelMat, m_position);
	modelMat = glm::rotate(modelMat,
		glm::radians(angleDegree), rotAxis);
	m_shaderProgram->Use();

	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID, "ModelMat"),
		1, GL_FALSE,
		glm::value_ptr(modelMat));
}

void SkinMeshRenderer::processNode(aiNode* node)
{
	// process all the node’s meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = m_scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i]);
	}
}

SkinMesh SkinMeshRenderer::processMesh(aiMesh* mesh)
{
	std::vector<SkinMeshVertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		SkinMeshVertex vertex;

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
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertex.BoneIDs = glm::vec4(0.0f);
		vertex.BoneWeights = glm::vec4(0.0f);

		vertices.push_back(vertex);
	}
	// bone section
	if (mesh->mNumBones>0) {

		for (GLuint i = 0; i < mesh->mNumBones; i++) {
			aiBone* bone = mesh->mBones[i];
			GLuint BoneIndex = 0;

			if (m_boneInfo.find(bone->mName.C_Str()) == m_boneInfo.end()) {
				glm::mat4 m = SkinMeshVertex::AssimpToGlmMatrix(bone->mOffsetMatrix);
				BoneIndex = m_boneCount;
				m_boneCount++;
				m_boneInfo[bone->mName.C_Str()] = { BoneIndex, m };
			}
			else {
				BoneIndex = m_boneInfo[bone->mName.C_Str()].first;
			}
			//loop through each vertex that have that bone
			for (GLuint j = 0; j < bone->mNumWeights; j++) {
				GLuint id = bone->mWeights[j].mVertexId;
				float weight = bone->mWeights[j].mWeight;
				
				for (GLuint vecIndex = 0; vecIndex < 4; vecIndex++) {
					if (vertices[id].BoneWeights[vecIndex] == 0.0) {
						vertices[id].BoneIDs[vecIndex] = BoneIndex;
						vertices[id].BoneWeights[vecIndex] = weight;
						break;
					}
				}
				
			}
		}
		// normalize vertexWeights
		for (GLuint i = 0; i < vertices.size(); i++) {
			glm::vec4& boneWeights = vertices[i].BoneWeights;
			float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
			if (totalWeight > 0.0f) {
				vertices[i].BoneWeights = glm::vec4(
					boneWeights.x / totalWeight,
					boneWeights.y / totalWeight,
					boneWeights.z / totalWeight,
					boneWeights.w / totalWeight
				);
			}
		}

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
		aiMaterial* material = m_scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(),
			specularMaps.end());
	}
	return SkinMesh(vertices, indices, textures);
}

std::vector<Texture> SkinMeshRenderer::loadMaterialTextures(aiMaterial* mat, aiTextureType type,
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

void SkinMeshRenderer::readSkeleton(Bone& boneOutput, aiNode* node) {

	if (m_boneInfo.find(node->mName.C_Str()) != m_boneInfo.end()) { // if node is actually a bone
		boneOutput.name = node->mName.C_Str();
		boneOutput.id = m_boneInfo[boneOutput.name].first;
		boneOutput.localTransForm = SkinMeshVertex::AssimpToGlmMatrix(node->mTransformation);

		for (int i = 0; i < node->mNumChildren; i++) {
			Bone child;
			readSkeleton(child, node->mChildren[i]);

			if(child.name != "")
			boneOutput.children.push_back(child);
		}
	}
	else { // find bones in children
		for (int i = 0; i < node->mNumChildren; i++) {
			readSkeleton(boneOutput, node->mChildren[i]);
		}
	}
}

void SkinMeshRenderer::loadAnimation(Animation& animation) {
	//loading  first Animation
	if (m_hasAnimation) {

		aiAnimation* anim = m_scene->mAnimations[0];

		if (anim->mTicksPerSecond != 0.0f)
			animation.ticksPerSecond = anim->mTicksPerSecond;
		else
			animation.ticksPerSecond = 1;


		animation.duration = anim->mDuration * animation.ticksPerSecond;
		animation.boneTransforms = {};

		//std::cout << animation.duration << std::endl;

		//load positions rotations and scales for each bone
		// each channel represents each bone
		for (int i = 0; i < anim->mNumChannels; i++) {
			aiNodeAnim * channel = anim->mChannels[i];
			//std::cout << "->>" << channel->mNodeName.C_Str() << std::endl;

			if (m_boneInfo.find(channel->mNodeName.C_Str()) != m_boneInfo.end()) {

				BoneTransformTrack track;
				for (int j = 0; j < channel->mNumPositionKeys; j++) {
					track.positionTimestamps.push_back(channel->mPositionKeys[j].mTime);
					track.positions.push_back(SkinMeshVertex::AssimpToGlmVec3(channel->mPositionKeys[j].mValue));
				}
				for (int j = 0; j < channel->mNumRotationKeys; j++) {
					track.rotationTimestamps.push_back(channel->mRotationKeys[j].mTime);
					track.rotations.push_back(SkinMeshVertex::AssimpToGlmQuat(channel->mRotationKeys[j].mValue));

				}
				for (int j = 0; j < channel->mNumScalingKeys; j++) {
					track.scaleTimestamps.push_back(channel->mScalingKeys[j].mTime);
					track.scales.push_back(SkinMeshVertex::AssimpToGlmVec3(channel->mScalingKeys[j].mValue));

				}
				if (track.positionTimestamps.size() > 1 || 
					track.rotationTimestamps.size() > 1 || 
					track.scaleTimestamps.size() > 1) {
					animation.boneTransforms[channel->mNodeName.C_Str()] = track;
					//std::cout << channel->mNodeName.C_Str() << std::endl;
				}

			}

		}

	}
	
}
std::pair<GLuint, float>  SkinMeshRenderer::getTimeFraction(
	std::vector<float>& times,
	float elapseTime) {

	GLuint segment = 0;
	while (elapseTime > times[segment])
		segment++;

	//std::cout << times.size()<<"   -> " << segment << std::endl;

	float start = times[segment - 1];
	float end = times[segment];
	float frac = (elapseTime - start) / (end - start);
	return { segment, frac };
}
void SkinMeshRenderer::getPose(
	Bone& skeletion, 
	float elapsedTime, 
	std::vector<glm::mat4>& output, 
	glm::mat4& parentTransform) 
{
	if (m_hasAnimation) {

		if (m_animation.boneTransforms.find(skeletion.name) != m_animation.boneTransforms.end())
		{
			BoneTransformTrack btt = m_animation.boneTransforms[skeletion.name];
			//std::cout << skeletion.name << std::endl;
			elapsedTime = fmod(elapsedTime, m_animation.duration);
			std::pair<GLuint, float> fp;
			//calculate interpolated position
			fp = getTimeFraction(btt.positionTimestamps, elapsedTime);

			glm::vec3 position1 = btt.positions[fp.first - 1];
			glm::vec3 position2 = btt.positions[fp.first];

			glm::vec3 position = glm::mix(position1, position2, fp.second);

			//calculate interpolated rotation
			fp = getTimeFraction(btt.rotationTimestamps, elapsedTime);
			glm::quat rotation1 = btt.rotations[fp.first - 1];
			glm::quat rotation2 = btt.rotations[fp.first];

			glm::quat rotation = glm::slerp(rotation1, rotation2, fp.second);

			//calculate interpolated scale
			fp = getTimeFraction(btt.scaleTimestamps, elapsedTime);
			glm::vec3 scale1 = btt.scales[fp.first - 1];
			glm::vec3 scale2 = btt.scales[fp.first];

			glm::vec3 scale = glm::mix(scale1, scale2, fp.second);

			glm::mat4 positionMat = glm::mat4(1.0),
				scaleMat = glm::mat4(1.0);


			// calculate localTransform
			positionMat = glm::translate(positionMat, position);
			glm::mat4 rotationMat = glm::toMat4(rotation);
			scaleMat = glm::scale(scaleMat, scale);
			glm::mat4 localTransform = positionMat * rotationMat * scaleMat;
			glm::mat4 globalTransform = parentTransform * localTransform;

			output[skeletion.id] = m_globalInverseTransform 
				* globalTransform * m_boneInfo[skeletion.name].second;
			//update values for children bones
			for (Bone& child : skeletion.children) {
				getPose(child, elapsedTime, output, globalTransform);
			}

			
		}
		else {

			glm::mat4 globalTransform = parentTransform * skeletion.localTransForm;
			//update values for children bones
			for (Bone& child : skeletion.children) {
				getPose(child, elapsedTime, output, globalTransform);
			}

		}
	}
}

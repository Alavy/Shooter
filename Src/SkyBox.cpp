#include"SkyBox.h"

SkyBox::SkyBox(Camera* camera)
{
	m_shaderProgram = new ShaderProgram("cubemap.vert", "cubemap.frag");
	/*
	std::vector<std::string> faces=
	{
			"Assets/2D/Skybox1/right.jpg",
			"Assets/2D/Skybox1/left.jpg",
			"Assets/2D/Skybox1/bottom.jpg",
			"Assets/2D/Skybox1/top.jpg",
			"Assets/2D/Skybox1/front.jpg",
			"Assets/2D/Skybox1/back.jpg"
	};
	*/
	std::vector<std::string> faces =
	{
			"Assets/2D/Skybox2/+X.png",
			"Assets/2D/Skybox2/-X.png",
			"Assets/2D/Skybox2/-Y.png",
			"Assets/2D/Skybox2/+Y.png",
			"Assets/2D/Skybox2/+Z.png",
			"Assets/2D/Skybox2/-Z.png"
	};
	std::vector<GLfloat> vertices = {
		// positions          
		   -1.0f,  1.0f, -1.0f,
		   -1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
		   -1.0f,  1.0f, -1.0f,

		   -1.0f, -1.0f,  1.0f,
		   -1.0f, -1.0f, -1.0f,
		   -1.0f,  1.0f, -1.0f,
		   -1.0f,  1.0f, -1.0f,
		   -1.0f,  1.0f,  1.0f,
		   -1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

		   -1.0f, -1.0f,  1.0f,
		   -1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
		   -1.0f, -1.0f,  1.0f,

		   -1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
		   -1.0f,  1.0f,  1.0f,
		   -1.0f,  1.0f, -1.0f,

		   -1.0f, -1.0f, -1.0f,
		   -1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
		   -1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
	};
	m_cubeMapTextureID = loadCubemap(faces);
	m_camera = camera;

	m_vao = new VAO;
	m_vao->Bind();
	m_vbo = new VBO(vertices.data(), vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
	// vertex attribute configuration 
	//layout,numOfValues,type, num Of Bytes each vetex,offset in bytes
	m_vao->LinkAttrib(*m_vbo, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), (void*)0);


	m_vbo->Bind();
	m_vao->UnBind();
	m_vbo->UnBind();

	Move(glm::vec3(0, 0, 0), 180, glm::vec3(0, 0, 1));
}

void SkyBox::Draw(float elapsedTime, float deltaTime)
{
	glDepthFunc(GL_LEQUAL);
	m_shaderProgram->Use();
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID,
		"projection"),
		1, GL_FALSE,
		glm::value_ptr(m_camera->ReturnPerpectiveMat()));
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID,
		"view"),
		1, GL_FALSE,
		glm::value_ptr(
			glm::mat4(
				glm::mat3(m_camera->ReturnViewMat())
			)
		)
	);
	m_vao->Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	m_vao->UnBind();
	glDepthFunc(GL_LESS);
}
void SkyBox::Move(glm::vec3 pos, float angleDegree, glm::vec3 rotAxis)
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
GLuint SkyBox::loadCubemap(std::vector<std::string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height,
			&nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap failed to load at path: " << faces[i]
				<< std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
		GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
		GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
		GL_CLAMP_TO_EDGE);
	return textureID;
}

void SkyBox::CleanUp()
{
	m_vao->Delete();
	m_vbo->Delete();
	m_shaderProgram->Delete();
	glDeleteTextures(1, &m_cubeMapTextureID);

	delete m_vao;
	delete m_vbo;
	delete m_shaderProgram;
}

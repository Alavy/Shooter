#include"SkinMesh.h"

SkinMesh::SkinMesh(std::vector<SkinMeshVertex> vertices, std::vector<GLuint> indices,
	std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	
	m_vao = new VAO;
	m_vao->Bind();
	m_vbo = new VBO(vertices.data(), vertices.size() * sizeof(SkinMeshVertex), GL_STATIC_DRAW);
	// pos
	m_vao->LinkAttrib(*m_vbo, 0, 3, GL_FLOAT, sizeof(SkinMeshVertex), (void*)0);
	// normal
	m_vao->LinkAttrib(*m_vbo, 1, 3, GL_FLOAT, sizeof(SkinMeshVertex),(void*)offsetof(SkinMeshVertex, Normal));
	// texcoords
	m_vao->LinkAttrib(*m_vbo, 2, 2, GL_FLOAT, sizeof(SkinMeshVertex),(void*)offsetof(SkinMeshVertex, TexCoords));
	// boneIds
	m_vao->LinkAttrib(*m_vbo, 3, 4, GL_FLOAT, sizeof(SkinMeshVertex), (void*)offsetof(SkinMeshVertex, BoneIDs));
	// bone weights
	m_vao->LinkAttrib(*m_vbo, 4, 4, GL_FLOAT, sizeof(SkinMeshVertex), (void*)offsetof(SkinMeshVertex, BoneWeights));

	m_ebo = new EBO(indices.data(), indices.size() * sizeof(GLuint));

	m_ebo->Bind();
	m_vbo->Bind();
	m_vao->UnBind();
	m_vbo->UnBind();
	m_ebo->UnBind();

}

void SkinMesh::Draw(ShaderProgram& shader)
{
	shader.Use();

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate texture unit first
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].Type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		shader.SetFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}
	glActiveTexture(GL_TEXTURE0);
	// draw mesh
	m_vao->Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	m_vao->UnBind();
}

void SkinMesh::CleanUp()
{
	m_vao->Delete();
	m_vbo->Delete();
	m_ebo->Delete();

	delete m_vao;
	delete m_vbo;
	delete m_ebo;
}

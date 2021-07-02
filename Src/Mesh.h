#pragma once
#include<glad/glad.h>
#include<glm/glm.hpp>

#include<iostream>
#include<vector>

#include"Vertex.h"
#include"Texture.h"
#include"ShaderProgram.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

class Mesh {
public:

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices,
		std::vector<Texture> textures);
	void Draw(ShaderProgram& shader);
	void CleanUp();

private:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	// render data
	VAO* m_vao;
	VBO* m_vbo;
	EBO* m_ebo;
};

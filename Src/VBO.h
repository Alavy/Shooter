#pragma once
#include<glad/glad.h>
#include"Vertex.h"
#include"SkinMeshVertex.h"

class VBO
{
public:
	VBO(GLfloat* data, GLsizeiptr size, GLenum drawType);
	VBO(SkinMeshVertex* data, GLsizeiptr size, GLenum drawType);
	VBO(Vertex* data, GLsizeiptr size, GLenum drawType);

	~VBO();
	void Bind();
	void UnBind();
	void Delete();
	void PassData(GLfloat* data, GLsizeiptr size);

private:
	GLuint ID;
	GLenum m_drawType;

};

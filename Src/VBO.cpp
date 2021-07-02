#include"VBO.h"

VBO::VBO(GLfloat* data,GLsizeiptr size,GLenum drawType) {
	m_drawType = drawType;

	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, drawType);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
VBO::VBO(Vertex* data, GLsizeiptr size, GLenum drawType) {
	m_drawType = drawType;

	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, drawType);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
VBO::VBO(SkinMeshVertex* data, GLsizeiptr size, GLenum drawType) {
	m_drawType = drawType;

	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, drawType);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::PassData(GLfloat* data, GLsizeiptr size)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, m_drawType);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
VBO::~VBO() 
{
	glDeleteBuffers(1, &ID);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}



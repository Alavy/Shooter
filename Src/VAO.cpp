#include"VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.UnBind();
}
void VAO::LinkIAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribIPointer(layout, numComponents, GL_INT, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.UnBind();
}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

// Unbinds the VAO
void VAO::UnBind()
{
	glBindVertexArray(0);
}

// Deletes the VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
VAO::~VAO() {
	glDeleteVertexArrays(1, &ID);
}

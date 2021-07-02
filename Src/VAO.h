#pragma once
#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	VAO();
	void Bind();
	void UnBind();
	void Delete();
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void LinkIAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLsizeiptr stride, void* offset);
	~VAO();

private:
	GLuint ID;

};
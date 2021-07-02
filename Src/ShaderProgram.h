#pragma once
#include"FileReader.h"
#include<string>
#include<glad/glad.h>

class ShaderProgram
{
public:
	GLuint ProgramID=-1;
	ShaderProgram(const char* vertexFilePath,const char* fragmentFilePath);
	~ShaderProgram();
	void Use();
	void SetFloat(const char* name,GLfloat value);
	void Delete();

private:
	GLuint vertexShaderID=-1;
	GLuint fragmentShaderID=-1;
};
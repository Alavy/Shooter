#include"ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* vertexFilePath, const char* fragmentFilePath)
{
	/// Vertex shader compilation
	std::string vertexCode = GetFileContent(vertexFilePath);
	const char * vertexShaderSourceCode = vertexCode.c_str();
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1,&vertexShaderSourceCode, NULL);
	glCompileShader(vertexShaderID);

	int successVertex;
	char infoLogVertex[512];
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &successVertex);

	if (!successVertex)
	{
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLogVertex);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLogVertex << std::endl;
		return;
	}

	/// Fragment shader compilation
	std::string fragCode = GetFileContent(fragmentFilePath);
	const char* fragmentShaderSourceCode = fragCode.c_str();
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSourceCode, NULL);
	glCompileShader(fragmentShaderID);

	int successFragment;
	char infoLogFragment[512];
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &successFragment);

	if (!successFragment)
	{
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLogFragment);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLogFragment << std::endl;
		return;
	}

	/// shader Program
	ProgramID = glCreateProgram();

	glAttachShader(ProgramID, vertexShaderID);
	glAttachShader(ProgramID, fragmentShaderID);
	glLinkProgram(ProgramID);
	// delete the shaders
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(ProgramID);

}

void ShaderProgram::Use()
{
	glUseProgram(ProgramID);

}

void ShaderProgram::SetFloat(const char* name, GLfloat value)
{
	glUniform1f(glGetUniformLocation(ProgramID, name), value);
}

void ShaderProgram::Delete()
{
	glDeleteProgram(ProgramID);

}

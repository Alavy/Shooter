#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include<iostream>
#include<map>
#include<vector>

#include"ShaderProgram.h"



class TextRenderer
{
public:
	struct Character {
		unsigned int TextureID; // ID handle of the glyph texture
		glm::ivec2   Size;      // Size of glyph
		glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
		unsigned int Advance;   // Horizontal offset to advance to next glyph
	};
	TextRenderer(const char* fontPath,GLuint width,GLuint height);
	void CleanUp();
	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

private:
	GLuint VAO, VBO;

	ShaderProgram* m_shaderProgram = nullptr;
	std::map<GLchar, Character> Characters;
	glm::mat4 m_projection = glm::mat4(1.0f);

};
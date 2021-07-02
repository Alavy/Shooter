#pragma once
#include<glad/glad.h>
#include<iostream>
#include"stb_image.h"


class Texture2D
{
public:
	Texture2D(const char * filePath);
	void Bind() const;
	void UnBind() const;
	void Delete() const;
	int Width() const { return m_width; };
	int Height() const { return m_height; };

private:
	int m_width=-1, m_height=-1, m_nrChannels=-1;
	GLuint ID;

};
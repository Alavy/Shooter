#pragma once
#include<glad/glad.h>
#include<iostream>
#include"stb_image.h"

struct Texture {
	unsigned int ID;
	std::string Type;
	std::string Path;
	static GLuint TextureFromFile(std::string filePath, std::string directrory);
};


#include "Texture.h"

GLuint Texture::TextureFromFile(std::string filePath, std::string directory)
{
	GLuint testureId;
	int width;
	int height;
	int numChannel;

	filePath = directory + '/' + filePath;
	
	// load and generate the texture
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height,
		&numChannel, 0);
	if (data)
	{
		glGenTextures(1, &testureId);

		GLenum format = GL_RGBA;
		if (numChannel == 1)
			format = GL_RED;
		else if (numChannel == 3)
			format = GL_RGB;
		else if (numChannel == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, testureId);
		
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
			GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//free up memory
		stbi_image_free(data);

	}
	else
	{
		std::cout << "Failed to load texture at "<<filePath << std::endl;
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
    return testureId;
}

#define STB_IMAGE_IMPLEMENTATION
#include "textures.h"

#define MAX(a,b) ((a)>(b)?(a):(b))

GLuint textureFromFile(const char * filename)
{
	int width, height;
	int numChannels;
	//	Tell stb_image to require 4 channels
	unsigned char * data = stbi_load(filename,&width,&height,&numChannels,4);
	if(!data)
	{
		setError(ERR_MESG,"Error retrieving data from \"%s\": %s",filename,stbi_failure_reason());
		return 0;
	}
	if(numChannels != 4 && numChannels != 3)
	{
		setError(ERR_MESG,"stb_image returned %d channels... (expected 3 or 4)",numChannels);
		return 0;
	}

	GLuint texture;
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTextureParameterfv(texture, GL_TEXTURE_BORDER_COLOR, (vec4){1.0f,0.0f,0.0f,1.0f});
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int mips = 1 + floor(log2(MAX(width,height)));
	if(numChannels == 4)
	{
		glTextureStorage2D(texture, mips, GL_RGBA8, width, height);
		glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		glTextureStorage2D(texture, mips, GL_RGB8, width, height);
		glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	glGenerateTextureMipmap(texture);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D,0);

	return texture;
}
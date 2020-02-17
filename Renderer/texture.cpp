#include "texture.h"



texture::texture()
{
	width = 0;
	height = 0;
	textureID = 0;
}

//This function loads the image and returns
unsigned char* texture::loadImage(const char* fileLoc, int* width, int* height) {
	
	image = SOIL_load_image(fileLoc, width, height, nullptr, SOIL_LOAD_RGBA);
	if (!image) {
		std::cout << "Error! Image load failed at " << fileLoc << std::endl;
		return nullptr;
	}
	//Update width and height
	this->width = *width;
	this->height = *height;
	return image;
	
}

//Use the texture
void texture::useTexture(GLuint textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

//This function loads the texture directly
void texture::loadTexture(std::string fileLoc) {
	//Load directly into texture
	const char* fileLoc_c = fileLoc.c_str();
	this->textureID = SOIL_load_OGL_texture(fileLoc_c, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS |SOIL_FLAG_INVERT_Y);

	if (!textureID) {
		const char* error = SOIL_last_result();
		std::cout << "Error! Image load failed at " << fileLoc << std::endl;
		std::cout << error << std::endl;
		return;
	}

	//Generate texture buffer and bind
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	   
	//Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::clearTexture() {
	glDeleteTextures(1, &textureID);
	width = 0;
	height = 0;
	image = NULL;
	textureID = 0;
}

texture::~texture()
{
	clearTexture();
}

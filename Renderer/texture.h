#pragma once
#include<stdlib.h>
#include<GL/glew.h>
#include<iostream>
#include<SOIL.h>
#include<string>
#include<vector>

//This class handles texture related operations like texture loading
class texture
{
public:
	texture();
	unsigned char* loadImage(const char* fileLoc, int* width, int* height);
	void loadTexture(std::string fileLoc);
	void useTexture(GLuint textureUnit);
	void clearTexture();
	~texture();

	//Public vars
	unsigned char*	image;
	std::string		path;
	int				width, height;
	GLuint			textureID;
};


#pragma once
#include <GL/glew.h>
#include<glm/glm.hpp>
#include<vector>

//Light base class


class light
{
public:
	light();
	light(glm::vec4 color, glm::float32 intensity);
	virtual glm::vec4 getColor();
	virtual GLfloat   getIntensity();
	virtual void getUniformLocs(GLuint shaderProgram, std::vector<const char*> parameters) = 0;
	virtual void activateLight() = 0;
	~light();

protected:
	glm::vec4			color;
	glm::float32		intensity;
};


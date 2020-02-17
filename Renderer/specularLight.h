#pragma once
#include "light.h"
class specularLight :
	public light
{
public:
	specularLight();
	specularLight(glm::vec4 color, glm::float32 intensity, glm::vec4 lightPos);
	void getUniformLocs(GLuint shaderProgram, std::vector<const char*> parameters);
	void updatePosition(glm::vec3 newLocation);
	void activateLight();
	~specularLight();

private:
	glm::vec4		specularLightPos;
	glm::float32	specularLightSpecularFactor;
	GLuint			colorUniformLoc;
	GLuint			intensityUniformLoc;
	GLuint			positionUniformLoc;
	GLuint			specularUniformLoc;
};


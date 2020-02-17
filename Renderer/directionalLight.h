#pragma once
#include "light.h"
class directionalLight :
	public light
{
public:
	directionalLight();
	directionalLight(glm::vec4 color, glm::float32 intensity, glm::vec4 lightDirection);
	void getUniformLocs(GLuint shaderProgram, std::vector<const char*> parameters);
	void activateLight();
	~directionalLight();

private:
	glm::vec4	lightSourceDirection;
	GLuint		colorUniformLoc;
	GLuint		intensityUniformLoc;
	GLuint		lightDirUniformLoc;
};


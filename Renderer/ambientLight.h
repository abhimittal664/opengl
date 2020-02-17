#pragma once
#include "light.h"
class ambientLight :
	public light
{
public:
	ambientLight();
	ambientLight(glm::vec4 color, glm::float32 intensity);
	void getUniformLocs(GLuint shaderProgram, std::vector<const char*> parameters);
	void activateLight();
	~ambientLight();

private:
	GLuint colorLoc;
	GLuint intensityLoc;
};


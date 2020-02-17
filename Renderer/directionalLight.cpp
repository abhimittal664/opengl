#include "directionalLight.h"


//Default intensity is 0. Must provide intensity and position
directionalLight::directionalLight()
	:	light{glm::vec4(1.f,1.f,1.f,1.f), glm::float32(0.f)},
		lightSourceDirection{glm::vec4(0.f,0.f,0.f,0.f)}
{
}

directionalLight:: directionalLight(glm::vec4 color, glm::float32 intensity, glm::vec4 lightDirection)
	:	light{color, intensity},
		lightSourceDirection{lightDirection}	{
}

//Get uniform Locations from the shader Program
//1. Pos	2. Color	3. Intensity
void directionalLight::getUniformLocs(GLuint shaderProgram, std::vector<const char*> parameters) {
	colorUniformLoc = glGetUniformLocation(shaderProgram, parameters[0]);
	intensityUniformLoc = glGetUniformLocation(shaderProgram, parameters[1]);
	lightDirUniformLoc = glGetUniformLocation(shaderProgram, parameters[2]);
}

void directionalLight::activateLight() {
	glUniform4f(lightDirUniformLoc, lightSourceDirection.x, lightSourceDirection.y, lightSourceDirection.z, lightSourceDirection.w);
	glUniform4f(colorUniformLoc, color.x, color.y, color.z, color.w);
	glUniform1f(intensityUniformLoc, intensity);
}

directionalLight::~directionalLight()
{
}

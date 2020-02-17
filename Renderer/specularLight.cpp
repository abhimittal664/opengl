#include "specularLight.h"


//Provided for completeness. Must use parameterized constructor
specularLight::specularLight()
	:	light{glm::vec4(1.f,1.f,1.f,1.f), glm::float32(0.f)},
		specularLightPos{glm::vec4(0.f,0.f,0.f,0.f)},
		specularLightSpecularFactor{glm::float32(0.f)}
{
}

specularLight::specularLight(glm::vec4 color, glm::float32 intensity, glm::vec4 lightPos)
	:	light{color, intensity},
		specularLightPos{lightPos}
{

}

//1. Color	2. Intensity	3. Position		4. Specular Factor
void specularLight::getUniformLocs(GLuint shaderProgram, std::vector<const char*> parameters) {
	colorUniformLoc = glGetUniformLocation(shaderProgram, parameters[0]);
	intensityUniformLoc = glGetUniformLocation(shaderProgram, parameters[1]);
	positionUniformLoc = glGetUniformLocation(shaderProgram, parameters[2]);
}

void specularLight::updatePosition(glm::vec3 newLocation) {
	this->specularLightPos = glm::vec4(newLocation, 1.f);
}

void specularLight::activateLight() {
	glUniform4f(colorUniformLoc, color.x, color.y, color.z, color.w);
	glUniform1f(intensityUniformLoc, intensity);
	glUniform4f(positionUniformLoc, specularLightPos.x, specularLightPos.y, specularLightPos.z, specularLightPos.w);
}

specularLight::~specularLight()
{
}

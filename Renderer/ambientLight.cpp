#include "ambientLight.h"



ambientLight::ambientLight()
	: light{glm::vec4(1.f,1.f,1.f,1.f), glm::float32(1.f)}
{

}

ambientLight::ambientLight(glm::vec4 color, glm::float32 intensity) 
	: light{color, intensity} {
	
}

//This function gets uniform locations for light parameters
void ambientLight::getUniformLocs(GLuint shaderProgram, std::vector<const char*> parameters) {
	colorLoc = glGetUniformLocation(shaderProgram, parameters[0]);
	intensityLoc = glGetUniformLocation(shaderProgram, parameters[1]);
}

//This function binds the light to Frag shader uniform
void ambientLight::activateLight() {
	glUniform4f(colorLoc, color.x, color.y, color.z, color.w);
	glUniform1f(intensityLoc, intensity);
}

ambientLight::~ambientLight()
{
}

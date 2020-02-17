#include "light.h"

light::light()
{
	//Default color is white. Default intensity is 1
	this->color = glm::vec4(1.f, 1.f, 1.f, 1.f);
	this->intensity = 1.f;
}

light::light(glm::vec4 color, glm::float32 intensity)
	:	color{ color }, 
		intensity{intensity} {
	
}

glm::vec4 light::getColor() {
	return (color);
}

GLfloat  light::getIntensity() {
	return (intensity);
}

light::~light()
{
}

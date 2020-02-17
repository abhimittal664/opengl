#include "mouse.h"



mouse::mouse(double scrollSpeed)
	: scrollSpeed{scrollSpeed}
{
}
glm::vec3 mouse::getMousePosition() {
	return(glm::vec3(float(this->xPos), float(this->yPos), float(this->zPos)));
}

void mouse::updateCursorPosition(GLFWwindow * window, double xOffset, double yOffset, double xPos, double yPos, int WIDTH, int HEIGHT)
{
	double temp_x, temp_y;
	//First Check if Left mouse button is pressed
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		//Get x and y coordinates and scale to window dimension
		xPosPrev = this->xPos; yPosPrev = this->yPos; zPosPrev = this->zPos;
		//X and Y
		glfwGetCursorPos(window, &temp_x, &temp_y);
		temp_x = temp_x - (double)(WIDTH/2);
		temp_y = -(temp_y - (double)(HEIGHT/2));
		this->xPos = temp_x / (double(WIDTH));
		this->yPos = temp_y / (double(HEIGHT));
	
		std::cout << this->xPos << " " << this->yPos << " " << this->zPos << std::endl;
	}

	//Scroll
	if (yOffset > 0) {
		this->zPos = this->zPos + scrollSpeed;
		std::cout << this->xPos << " " << this->yPos << " " << this->zPos << std::endl;
	}
	else if (yOffset < 0) {
		this->zPos = this->zPos - scrollSpeed;
		std::cout << this->xPos << " " << this->yPos << " " << this->zPos << std::endl;
	}
}


mouse::~mouse()
{
}

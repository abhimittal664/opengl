#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <stdlib.h>
#include <iostream>

class mouse
{
public:
	mouse(double scrollSpeed);

	void updateCursorPosition(GLFWwindow* window, double xOffset, double yOffset, double xPos, double yPos, int WIDTH, int HEIGHT);
	glm::vec3 getMousePosition();
	~mouse();

private:
	double xPos, yPos, zPos;
	double xPosPrev, yPosPrev, zPosPrev;
	double scrollSpeed;
};


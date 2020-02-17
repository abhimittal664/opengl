#include "camera.h"



camera::camera()
{
	cur_x = 0;	cur_y = 0;	cur_z = 0;
	prev_x = 0;	prev_y = 0;	prev_z = 0;
	x_speed = 0.1f; y_speed = 0.1f; z_speed = 0.1f;
}

camera::camera(unsigned int x, unsigned int y, unsigned int z)
{
	cur_x = x; 	cur_y = y;	cur_z = z;
	prev_x = x;	prev_y = y;	prev_z = z;
	x_speed = 0.1f; y_speed = 0.1f; z_speed = 0.1f;
}

//This function monitors key presses and updates current camera location
void camera::updateLocation(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		//Reset
		 cur_x = 0;  cur_y = 0;  cur_z = 0;
		 prev_x = 0;  prev_y = 0;  prev_z = 0;
		 view_x = 0; view_y = 0; view_z = 0;
		 prev_view_x = 0; prev_view_y = 0; prev_view_z = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//Forward
		prev_z = cur_z;
		cur_z += z_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//Backward
		prev_z = cur_z;
		cur_z -= z_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		//Left
		prev_x = cur_x;
		cur_x -= x_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		//Right
		prev_x = cur_x;
		cur_x += x_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		//Up
		prev_y = cur_y;
		cur_y += y_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		//Down
		prev_y = cur_y;
		cur_y -= y_speed;
	}

}

glm::vec3 camera::getLocation() {
	return glm::vec3(cur_x, cur_y, cur_z);
}

void camera::getUniformLocs(GLuint shaderProgram, std::vector<const char*> parameters) {
	cameraPosUniformLoc = glGetUniformLocation(shaderProgram, parameters[0]);
}

void camera::bindCameraUniform() {
	glUniform4f(cameraPosUniformLoc, cur_x, cur_y, cur_z, 1.f);
}

camera::~camera()
{
}

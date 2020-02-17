#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class camera
{
public:
	camera();
	camera(unsigned int, unsigned int, unsigned int);
	void updateLocation(GLFWwindow*);
	void getUniformLocs(GLuint shaderProgram, std::vector<const char*> parameters);
	void bindCameraUniform();
	glm::vec3 getLocation();
	~camera();

	private:
	GLfloat cur_x, cur_y, cur_z, prev_x, prev_y, prev_z;
	GLfloat view_x, view_y, view_z, prev_view_x, prev_view_y, prev_view_z;
	GLfloat x_speed, y_speed, z_speed;
	GLuint	cameraPosUniformLoc;
};


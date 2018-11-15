#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstring>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<fstream>
using namespace std;

//Vertex buffer definition
struct vertex {
	glm::vec3 position;
	glm::vec3 color;
};
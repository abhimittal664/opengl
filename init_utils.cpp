#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<tuple>
#include"typedefs.cpp"
#include"init_utils_header.h"
#include<vector>
using namespace std;

//Helper function to read file
string readShaderFile(const char* filename) {
	ifstream input(filename);
	if (!input.good()) {
		cout << "File not found" << filename << "\n";
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(input),
		std::istreambuf_iterator<char>());
}

//Helper function to compile shaders
std::tuple<GLuint, GLuint> compile_shader(const char* vertex_shader, const char* fragment_shader) {
	//Compile shaders
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	return std::make_tuple(vs, fs);
}
GLuint createShaderProgram(GLuint vs, GLuint fs) {
	GLuint sp = glCreateProgram();
	glAttachShader(sp,vs);
	glAttachShader(sp,fs);
	glLinkProgram(sp);

	return sp;
 }

void populate_vertex_buffer(vertex* vertices, int n_vertices) {
	std::ofstream log;
	log.open("log2.txt");
	float count = 0;
	for (int x = 0; x < n_vertices; x++) {
		count = -0.5f + (1.f / n_vertices)*(float)x;
			vertices[x].position = glm::vec3((-0.5f + (1.f / n_vertices)*(float)x), 0, 0);
			vertices[x].color = glm::vec3(0.0, 0.0, 0.0);
			log << count << endl;

	}
}

void draw_geomtery(GLuint vao) {
	glDrawArrays(GL_TRIANGLES, 0, 3);

}

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
	int count = 0;
	std::ofstream log;
	log.open("log3.txt");
	for (int x = 0; x < n_vertices; x++) {
		for (int z = 0; z < n_vertices; z++) {
			vertices[count].position = glm::vec3(-0.475f + (1.f / n_vertices)*(x), 0, -0.5 + (1.f / n_vertices)*(z));
			vertices[count].color = glm::vec3(0.0, 0.0, 0.0);
			log << x << " " << z << " " << count << endl;
			count++;
		}
	}
	log.close();
}

void populate_index_buffer(GLuint* indices, int n_indices) {
	std::ofstream log;
	log.open("log2.txt");
	int count = 0;
	for (int i = 0; i < n_indices-1; i++) {
		for (int j = 0; j < n_indices-1	; j++) {
			int i0 = i * (n_indices) + j;	//base Vertex
			int i1 = i0 + 1;				//right of base 
			int i2 = i0 + n_indices;		//below base
			int i3 = i2 + 1;				//diagonal right to base

			//Assign indices				
			indices[count++] = i0 ; indices[count++] = i2 ; indices[count++] = i1 ; //First triangle
			indices[count++] = i1 ; indices[count++] = i2 ; indices[count++] = i3 ; //Second triangle
			log << i0 << " " << i1 << " " << i2 << " " << i3 <<" "<<count<< endl;
		}
	}
	log.close();
}

void draw_geomtery(GLuint vao) {
	glDrawArrays(GL_TRIANGLES, 0, 3);

}

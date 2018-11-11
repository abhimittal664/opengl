#pragma once
std::string readShaderFile(const char*);
std::tuple<GLuint, GLuint> compile_shader(const char*, const char*);
GLuint createShaderProgram(GLuint, GLuint);
void populate_vertex_buffer(vertex*, int);
void draw_geometry(GLuint);
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <cstring>
#include<stdlib.h>
#include<string>
#include<tuple>
#include "typedefs.cpp"
#include "init_utils_header.h"
#include<iostream>
#include<fstream>
#include <windows.h>
using namespace std;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hwnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL bQuit = FALSE;
	float theta = 0.0f;

	/* register window class */
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "GLSample";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


	if (!RegisterClassEx(&wcex))
		return 0;

	/* create main window */
	hwnd = CreateWindowEx(0,
		"GLSample",
		"OpenGL Sample",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);

	/* enable OpenGL for the window */
	EnableOpenGL(hwnd, &hDC, &hRC);

	//Init Glew
	glewExperimental = GL_TRUE;
	glewInit();

	//Set render mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Disable face culling
	glDisable(GL_CULL_FACE);

	//Create vertex  and index buffer 
	vertex vertices[400];
	GLuint indices[2166];

	//Populate vertex buffer;
	(void)populate_vertex_buffer(vertices, 20);
	
	//Populate index buffer
	(void)populate_index_buffer(indices, 20);

	//Create vbo for postion and color
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GLuint ib = 0;
	glGenBuffers(1, &ib);


	//Create vao for position and color
	GLuint vao = 0;
	glGenVertexArrays (1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(FLOAT), NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(FLOAT), (const GLvoid*)offsetof(vertex, color));
	glEnableVertexAttribArray(1);

	//Create Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	//Load VS and FS
	string vertex_shader_s = readShaderFile("vertex_shader.glsl");
	string fragment_shader_s = readShaderFile("fragment_shader.glsl");
	const char* vertex_shader = vertex_shader_s.c_str();
	const char* fragment_shader = fragment_shader_s.c_str();

	//Compile and link shaders
	GLuint vs, fs;
	tie(vs, fs) = compile_shader(vertex_shader, fragment_shader);

	//Create shader program
	GLuint sp = createShaderProgram(vs, fs);	

	//Create MVP matrices
	glm::mat4 V = glm::lookAt(glm::vec3(0.f, 2.5f, 0.f),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 1));
	glm::mat4 P = glm::perspective(glm::radians(45.0f),
								   (GLfloat)(4.f/3.f),
								   0.01f,
		50.0f);
	glm::mat4 M = glm::mat4(1.0f);
	glm::mat4 MVP = P * V*M;

	//Get MVP matrix id
	GLuint MatrixID = glGetUniformLocation(sp, "MVP");

	GLint testval, abort;
	abort = 0;
	std::ofstream log;
	log.open("log.txt");
	glGetShaderiv(vs, GL_COMPILE_STATUS, &testval);
	if (testval == GL_FALSE)
	{
		abort = 1;
		char infolog[1024];
		glGetShaderInfoLog(vs, 1024, NULL, infolog);
		log << "The vertex shader failed to compile with the error:" << endl << infolog << endl;
	}

	glGetShaderiv(fs, GL_COMPILE_STATUS, &testval);
	if (testval == GL_FALSE)
	{
		abort = 1;
		char infolog[1024];
		glGetShaderInfoLog(fs, 1024, NULL, infolog);
		log << "The fragment shader failed to compile with the error:" << endl << infolog << endl;
	}

	glGetProgramiv(sp, GL_LINK_STATUS, &testval);
	if (testval == GL_FALSE)
	{
		abort = 1;
		char infolog[1024];
		glGetProgramInfoLog(sp, 1024, NULL, infolog);
		log << "The program failed to compile with the error:" << endl << infolog << endl;
	}
	if (abort)
	{
		log << "errors occured, cannot continue, aborting." << endl;
		exit(-1);
	}
	
	GLfloat time = 0;
	GLfloat time_c = 0;

	/* program main loop */
	while (!bQuit)
	{
		/* check for messages */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			/* handle or dispatch messages */
			if (msg.message == WM_QUIT)
			{
				bQuit = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			/* OpenGL animation code goes here */

			glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			//Increment time counter
			++time_c;
			time = time_c / 1000.f;
			glUseProgram(sp);

			glBindVertexArray(vao);
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniform1f(glGetUniformLocation(sp, "time"), time);
			//glDrawArrays(GL_POINTS, 0, 400);
			glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(UINT), GL_UNSIGNED_INT, 0);
			//(void)draw_geometry(vao);
			//glPushMatrix();
			//glRotatef(theta, 0.0f, 0.0f, 1.0f);

			//glBegin(GL_TRIANGLES);

			//glColor3f(1.0f, 0.0f, 0.0f);   glVertex2f(0.0f, 1.0f);
			//glColor3f(0.0f, 1.0f, 0.0f);   glVertex2f(0.87f, -0.5f);
			//glColor3f(0.0f, 0.0f, 1.0f);   glVertex2f(-0.87f, -0.5f);

			//glEnd();

			//glPopMatrix();

			SwapBuffers(hDC);

			//theta += 1.0f;
			Sleep(1);
		}
	}

	/* shutdown OpenGL */
	DisableOpenGL(hwnd, hDC, hRC);

	/* destroy the window explicitly */
	DestroyWindow(hwnd);
	log.close();
	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		return 0;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
	}
	break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
	PIXELFORMATDESCRIPTOR pfd;

	int iFormat;

	/* get the device context (DC) */
	*hDC = GetDC(hwnd);

	/* set the pixel format for the DC */
	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(*hDC, &pfd);

	SetPixelFormat(*hDC, iFormat, &pfd);

	/* create and enable the render context (RC) */
	*hRC = wglCreateContext(*hDC);

	wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hwnd, hDC);
}

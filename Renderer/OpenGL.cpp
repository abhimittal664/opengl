// OpenGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "mesh.h"
#include <vector>
#include <cstring>
#include <string>
#include <fstream>
#include "camera.h"
#include <Windows.h>
#include <assimp/Importer.hpp>
#include "texture.h"

#include "mouse.h"
#include "ambientLight.h"
#include "directionalLight.h"
#include "specularLight.h"

//Screen Resolution
const GLint WIDTH = 800, HEIGHT = 600;

mouse myMouse(double(0.1));

//Vertex Shader
static const char* vertexShader = "C:\\Users\\Abhishek\\source\\repos\\OpenGL\\OpenGL\\vertexShader.glsl";

//Fragment Shader
static const char* fragmentShader = "C:\\Users\\Abhishek\\source\\repos\\OpenGL\\OpenGL\\fragmentShader.glsl";

//For Scroll, Mouse doesnt need to be pressed
void updateScrollPosition(GLFWwindow * window, double xOffset, double yOffset){
	myMouse.updateCursorPosition(window, xOffset, yOffset, 0, 0, WIDTH, HEIGHT);
}

void updateCursorPosition(GLFWwindow * window, double xPos, double yPos) {
	myMouse.updateCursorPosition(window, 0, 0, xPos, yPos, WIDTH, HEIGHT);
}
//Function Defs end

int main()
{
    //Init GLFW
	if (!glfwInit()) {
		std::cout << "GLFW Init Failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	//Setup window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//Create Window
	GLFWwindow* myWindow = glfwCreateWindow(WIDTH, HEIGHT, "My Window", NULL, NULL);
	if (!myWindow) {
		std::cout << "Main Window Creation Failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	//Get FrameBuffer Dimension
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(myWindow, &bufferWidth, &bufferHeight);

	//Set current window
	glfwMakeContextCurrent(myWindow);

	glewExperimental = GL_TRUE;

	//Init GLEW
	if (!glewInit()==GLEW_OK) {
		std::cout << "GLEW Init Failed" << std::endl;
		glfwDestroyWindow(myWindow);
		glfwTerminate();
		return 1;
	}
	glfwSetScrollCallback(myWindow, updateScrollPosition);
	glfwSetCursorPosCallback(myWindow, updateCursorPosition);
	//Enable Depth
	glEnable(GL_DEPTH_TEST);

	//Set Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	//Create geometry
	std::string modelPath = "E:\\Downloads\\Meshes\\golf_ball\\";
	std::string modelFile = "ball.obj";
	model* myModel = new model(modelPath, modelFile);
	myModel->compileShader(vertexShader, fragmentShader);
	myModel->buildBuffers();

	//Set MVP matrices
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));

	glm::mat4 projection;
	projection = glm::perspective(45.f, (glm::float32(bufferWidth) / glm::float32(bufferHeight)), 0.1f, 100.f);

	glm::mat4 view;
	view = glm::lookAt(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

	//Bind the ambient light to Frag Shader
	std::vector<const char*> ambientLightParameters = {
		"ambientLight_0.color",
		"ambientLight_0.intensity"
	};
	glm::vec4 ambientLightColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	glm::float32 ambientLightIntensity = glm::float32(0.5f);
	ambientLight* ambientLight_0 = new ambientLight(ambientLightColor,ambientLightIntensity);
	ambientLight_0->getUniformLocs(myModel->shaderProgram, ambientLightParameters);
	
	//Bind the directional Light to Frag Shader
	std::vector<const char*> directionalLightParams = {
		"directionalLight_0.color",
		"directionalLight_0.intensity",
		"directionalLight_0.direction"
	};
	glm::vec4 directionalLightColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	glm::vec4 directionalLightDir = glm::vec4(1.0f, 0.f, -1.f, 1.f);
	glm::float32 directionalLightIntensity = glm::float32(0.6f);
	directionalLight*  directionalLight_0 = new directionalLight(directionalLightColor, directionalLightIntensity, directionalLightDir);
	directionalLight_0->getUniformLocs(myModel->shaderProgram, directionalLightParams);
	
	//Point Light
	std::vector<const char*> specularLightParams = {
		"specularLight_0.color",
		"specularLight_0.intensity",
		"specularLight_0.position",
		"specularLight_0.specularFactor"
	};
	glm::vec4 specularLightColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	glm::float32 specularLightIntensity = glm::float32(0.3f);
	glm::vec4 specularLightPosition = glm::vec4(1.f, 0.f, 1.f, 1.f);
	specularLight* specularLight_0 = new specularLight(specularLightColor, specularLightIntensity, specularLightPosition);
	specularLight_0->getUniformLocs(myModel->shaderProgram, specularLightParams);

	//Create Camera
	camera* myCamera = new camera(0.f, 0.f, 2.f);
	glm::vec3 cameraLoc;
	std::vector<const char*> cameraParams = {
		"camera_0.cameraPos"
	};
	myCamera->getUniformLocs(myModel->shaderProgram, cameraParams);

	//Get uniform locations
	myModel->uniformModel = glGetUniformLocation(myModel->shaderProgram, "model");
	myModel->uniformProjection = glGetUniformLocation(myModel->shaderProgram, "projection");
	myModel->uniformView = glGetUniformLocation(myModel->shaderProgram, "view");
	
	//Main Loop
	while (!glfwWindowShouldClose(myWindow)) {
		//Handle User Input
		glfwPollEvents();

		//Set Default color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(myModel->shaderProgram);

		//Update camera location
		myCamera->updateLocation(myWindow);
		cameraLoc = myCamera->getLocation();
		myCamera->bindCameraUniform();

		//Update View matrix
		view = glm::lookAt(cameraLoc, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		
		//Update Point light location
		specularLight_0->updatePosition(myMouse.getMousePosition());

		//Bind uniforms
		ambientLight_0->activateLight();
		directionalLight_0->activateLight();
		specularLight_0->activateLight();
		glUniformMatrix4fv(myModel->uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(myModel->uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(myModel->uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		
		myModel->renderMesh(myModel->shaderProgram);
				
		glUseProgram(0);

		//Swap Buffer
		glfwSwapBuffers(myWindow);
		Sleep(10);
	}
}

#include <iostream>

#define GLEW_STATIC

#include "GL/glew.h" // included before glfw 
#include "GLFW/glfw3.h"

const char* APP_TITLE = "Shader Editor";

const int gWindowWidth = 800;
const int gWindowHeight = 600;


int main() 
{
	if (!glfwInit()) 
	{
		std::cerr << "GLFW Initialization Failed" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* pWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);

	if (pWindow == NULL)
	{
		std::cerr << "Failed to create Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(pWindow);

	// For enabling linking modern OpenGL
	glewExperimental = GL_TRUE;

	// Init Glew 

	if (glewInit() != GLEW_OK) 
	{
		std::cerr << "GLEW Initialization Failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	while (!glfwWindowShouldClose(pWindow)) 
	{
		glfwPollEvents();

		glfwSwapBuffers(pWindow); 
	}

	glfwTerminate();
	return 0;
}
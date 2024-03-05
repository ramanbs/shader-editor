#include <iostream>
#include <sstream>

#define GLEW_STATIC

#include "GL/glew.h" // included before glfw 
#include "GLFW/glfw3.h"
#include "OpenGLShaderProgram.h"

const char* APP_TITLE = "Shader Editor";

const int gWindowWidth = 800;
const int gWindowHeight = 600;

bool gFullScreen = false;
bool gWireFrame = false;

GLFWwindow* gWindow = NULL;

void glfw_onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mode);
void showFPS(GLFWwindow* window);

bool initOpenGL();

int main() 
{
	if (!initOpenGL()) 
	{
		std::cerr << "OpenGL intialization failed" << std::endl;
		return -1;
	}

	GLfloat vertices[] =
	{
		-0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
	     0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	GLuint indices[] =
	{
		0, 1, 2,   //tri0
		0, 2, 3   //tri1
	};

	GLuint vbo, ibo, vao;

	//position
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //GL_STATIC_DRAW - create once and use it a lot

	// Vertex Array Object - holds information for our buffer, when we draw. Instead of having multiple buffers for different information ,

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Need to tell vertex shader how the vertices are laid out
	// position 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // index, number of attributes, data type, normalized ? , stride, offset
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	OpenGLShaderProgram shaderProgram;
	shaderProgram.loadShaders("shaders/basic.vert", "shaders/basic.frag");


	while (!glfwWindowShouldClose(gWindow)) 
	{
		showFPS(gWindow);
		glfwPollEvents();

		
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();

		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(0);

		
		glfwSwapBuffers(gWindow); 
	}

	
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return 0;
}

bool initOpenGL() 
{
	if (!glfwInit())
	{
		std::cerr << "GLFW Initialization Failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	if (gFullScreen)
	{
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);

		if (pVmode != NULL)
		{
			gWindow = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE, pMonitor, NULL);
		}
	}
	else
	{
		gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	}

	if (gWindow == NULL)
	{
		std::cerr << "Failed to create Window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(gWindow);

	glfwSetKeyCallback(gWindow, glfw_onKeyPressed);

	// For enabling linking modern OpenGL
	glewExperimental = GL_TRUE;

	// Init Glew 

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW Initialization Failed" << std::endl;
		glfwTerminate();
		return false;
	}

	glClearColor(0.23f, 0.30f, 0.47f, 1.0f);

	return true;
}

void glfw_onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		gWireFrame =! gWireFrame;
		if (gWireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void showFPS(GLFWwindow* window) 
{
	static double previousSeconds = 0.0f;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // number of seconds since GLFW started, uses high resolution timer

	elapsedSeconds = currentSeconds - previousSeconds;

	// limit text update 4 times per second

	if (elapsedSeconds > 0.25f)
	{
		previousSeconds = currentSeconds;

		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		std::ostringstream outStringStream;
		outStringStream.precision(3);

		outStringStream << std::fixed << APP_TITLE << "  "
			<< "FPS : " << fps << "  "
			<< "FrameTime:  " << msPerFrame << " (ms)";

		glfwSetWindowTitle(window, outStringStream.str().c_str());

		frameCount = 0;
	}

	frameCount++;
}
#include <iostream>
#include <sstream>

#define GLEW_STATIC

#include "GL/glew.h" // included before glfw 
#include "GLFW/glfw3.h"

const char* APP_TITLE = "Shader Editor";

const int gWindowWidth = 800;
const int gWindowHeight = 600;

bool gFullScreen = false;

GLFWwindow* gWindow = NULL;

const GLchar* vertexShaderSrc =
"#version 330 core\n"
"layout(location = 0) in vec3 pos;"
"void main()"
"{"
"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"}";

const GLchar* fragShaderSrc =
"#version 330 core\n"
"out vec4 frag_color;"
"void main()"
"{"
"  frag_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);"
"}";

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
		0.0f,  0.5f, 0.0f, // Top
		0.5f, -0.5f, 0.0f, // Right
	   -0.5f, -0.5f, 0.0f // Left
	};

	GLuint vbo, vao;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //GL_STATIC_DRAW - create once and use it a lot

	// Vertex Array Object - holds information for our buffer, when we draw. Instead of having mutiple buffers for different information ,

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Need to tell vertex shader how the vertices are laid out

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);


	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShaderSrc, NULL);
	
	while (!glfwWindowShouldClose(gWindow)) 
	{
		showFPS(gWindow);
		glfwPollEvents();

		
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		
		glfwSwapBuffers(gWindow); 
	}

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
#include <iostream>
#include <sstream>

#define GLEW_STATIC

#include "GL/glew.h" // included before glfw 
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "OpenGLShaderProgram.h"
#include "Texture2D.h"
#include "Camera.h"

const char* APP_TITLE = "Shader Editor";

int gWindowWidth = 1024;
int gWindowHeight = 768;

bool gFullScreen = false;
bool gWireFrame = false;

const std::string  texture1Filename = "textures/demonslayer.png";

const std::string  texture2Filename = "textures/grid.jpg";

GLFWwindow* gWindow = NULL;


FPSCamera fpsCamera(glm::vec3(0.0f, 3.0f, 10.0f));

const double ZOOM_SENSITIVITY = -3.0f;
const float MOVE_SPEED = 5.0f;
const float MOUSE_SENSITIVITY = 0.1f;

void glfw_onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseMove(GLFWwindow* window, double posX, double posY);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);

void updateFPSCameraMouseInput(double elapsedTime);
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
        // position			// tex coords

		// front face
		-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		// back face
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		// left face
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		-1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		// right face
		 1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		// top face
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

		 // bottom face
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	};

	glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, -5.0f);
	glm::vec3 floorPos = glm::vec3(0.0f, -1.0f, 0.0f); 

	GLuint vbo, vao;

	//position
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //GL_STATIC_DRAW - create once and use it a lot

	// Vertex Array Object - holds information for our buffer, when we draw. Instead of having multiple buffers for different information ,

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Need to tell vertex shader how the vertices are laid out
	// position 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL); // index, number of attributes, data type, normalized ? , stride, offset
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	OpenGLShaderProgram shaderProgram;
	shaderProgram.loadShaders("shaders/basic.vert", "shaders/basic.frag");

	Texture2D texture1;
	texture1.loadTexture(texture1Filename, true);

	Texture2D texture2;
	texture2.loadTexture(texture2Filename, true);

	float cubeAngle = 0.0f;
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(gWindow)) 
	{
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		glfwPollEvents();
		updateFPSCameraMouseInput(deltaTime);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture1.bind(0);
		

		glm::mat4 model(1.0f), view(1.0f), projection(1.0f);


		model = glm::translate(model, cubePos);

		view = fpsCamera.getViewMatrix(); // need to look at construction of view matrix again

		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);

		shaderProgram.use();

		shaderProgram.setUniform("model", model);
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);

		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		texture2.bind(0);

		model = glm::translate(model, floorPos) * glm::scale(model, glm::vec3(10.0f, 0.01f, 10.0f));

		shaderProgram.setUniform("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glBindVertexArray(0);

		
		glfwSwapBuffers(gWindow); 

		lastTime = currentTime;
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
	glfwSetFramebufferSizeCallback(gWindow, glfw_OnFrameBufferSize);
	glfwSetCursorPosCallback(gWindow, glfw_onMouseMove);
	glfwSetScrollCallback(gWindow, glfw_onMouseScroll);

	//Hides and grabs cursor, unlimited movement
	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // disables cursor
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0f, gWindowHeight / 2.0f);

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
	glViewport(0, 0, gWindowWidth, gWindowHeight);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void glfw_onKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		gWireFrame =! gWireFrame;
		if (gWireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
void glfw_onMouseMove(GLFWwindow* window, double posX, double posY) 
{
	// Do nothing
}

void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
	double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

	fov = glm::clamp(fov, 1.0, 120.0);

	fpsCamera.setFOV((float)fov);
}

void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height) 
{
	gWindowWidth = width;
	gWindowHeight = height;

	glViewport(0, 0, gWindowWidth, gWindowHeight);
}

void updateFPSCameraMouseInput(double elapsedTime)
{
	//Camera orientation 
	double mouseX, mouseY;

	// Get the current mouse cursor position delta
	glfwGetCursorPos(gWindow, &mouseX, &mouseY);

	// Rotate the camera the difference in mouse distance from the center screen. Multiply this delay by a speed scaler 
	fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);

	// reset the mouse cursor to center of screen every frame since above calculation fo pitch and yaw is dependent on it
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

	// Camera FPS movement

	if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
	else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());

	// Strafe left/ right
	if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());
	else if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());

	// Up / Down
	if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
	else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());
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
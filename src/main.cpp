#include <iostream>
#include <sstream>

#define GLEW_STATIC

#include "GL/glew.h" // included before glfw 
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "OpenGLShaderProgram.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Mesh.h"

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

	// Model positions
	glm::vec3 modelPos[] = 
	{
		glm::vec3(-3.5f, 0.0f, 0.0f),	// crate1
		glm::vec3(3.5f, 0.0f, 0.0f),	// crate2
		glm::vec3(0.0f, 0.0f, -2.0f),	// robot
		glm::vec3(0.0f, 0.0f, 0.0f),	// floor
		glm::vec3(0.0f, 0.0f, 2.0f),	// pin
		glm::vec3(-2.0f, 0.0f, 2.0f)	// bunny
	};

	glm::vec3 modelScale[] = 
	{
		glm::vec3(1.0f, 1.0f, 1.0f),  //crate
		glm::vec3(1.0f, 1.0f, 1.0f),  //wood crate
		glm::vec3(1.0f, 1.0f, 1.0f),  //robot
		glm::vec3(10.0f, 1.0f, 10.0f),  //floor
		glm::vec3(0.1f, 0.1f, 0.1f),  //pin
		glm::vec3(0.7f, 0.7f, 0.7f)  //bunny
	};


	const int numModels = 6;
	Mesh mesh[numModels];
	Texture2D texture[numModels];


	mesh[0].loadOBJ("models/crate.obj");
	mesh[1].loadOBJ("models/woodcrate.obj");
	mesh[2].loadOBJ("models/robot.obj");
	mesh[3].loadOBJ("models/floor.obj");
	mesh[4].loadOBJ("models/bowling_pin.obj");
	mesh[5].loadOBJ("models/bunny.obj");
	
	texture[0].loadTexture("textures/crate.jpg", true);
	texture[1].loadTexture("textures/woodcrate_diffuse.jpg", true);
	texture[2].loadTexture("textures/robot_diffuse.jpg", true);
	texture[3].loadTexture("textures/tile_floor.jpg", true);
	texture[4].loadTexture("textures/AMF.tga", true);
	texture[5].loadTexture("textures/bunny_diffuse.jpg", true);
	

	OpenGLShaderProgram shaderProgram;
	shaderProgram.loadShaders("shaders/basic.vert", "shaders/basic.frag");



	float angle = 0.0f;
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(gWindow)) 
	{
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		glfwPollEvents();
		updateFPSCameraMouseInput(deltaTime);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0f), view(1.0f), projection(1.0f);

		view = fpsCamera.getViewMatrix(); // need to look at construction of view matrix again

		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);

		shaderProgram.use();

		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);


		for (int i = 0; i < numModels; i++)
		{
			model = glm::translate(glm::mat4(1.0f), modelPos[i]) * glm::scale(glm::mat4(1.0f), modelScale[i]);

			shaderProgram.setUniform("model", model);

			texture[i].bind(0);
			mesh[i].draw();
			texture[i].unbind(0);
		}
		
		glfwSwapBuffers(gWindow); 

		lastTime = currentTime;
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
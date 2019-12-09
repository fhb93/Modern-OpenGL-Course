#include <iostream>
#include <sstream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"


const char * APP_TITLE = "Introduction to Modern OpenGL - Hello Window 1";

const int gWindowWidth = 800;

const int gwindowHeight = 600;

bool gFullscreen = false;

// such callback function is important for key input event
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);

// good feedback on how it is performing
void showFPS(GLFWwindow * window);

int main(void)
{
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//avoiding obsolete function calls
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//for mac users
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow * pWindow = NULL;

	if (gFullscreen)
	{
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();

		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);

		if (pVmode != NULL)
		{
			pWindow = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE, pMonitor, NULL);
		}

	}
	else {
		pWindow = glfwCreateWindow(gWindowWidth, gwindowHeight, APP_TITLE, NULL, NULL);
	}

	if (pWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		
		glfwTerminate();

		return -2;
	}

	// gives focus to the newly created window
	glfwMakeContextCurrent(pWindow);

	glfwSetKeyCallback(pWindow, glfw_onKey);

	//important for modern openGL correct setup
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW initialization failed" << std::endl;
		
		return -3;
	}

	//Main Loop
	while (!glfwWindowShouldClose(pWindow))
	{
		showFPS(pWindow);

		glfwPollEvents();

		//OpenGL is a big state machine 
		// it keeps the current state until we change it
		// now, for a first test, we will change the color state
		// it's good to paint with a color other than plain black
		// because if something wrong happens, black will appears
		// against the other color, and such effect fits as a
		// convenient visual warning

		glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//2 canvas that show the same picture
		// one is back buffer, other is front buffer
		// this swap function keeps changing buffers,
		// it's called double buffer
		// and is used to avoid image flickering
		glfwSwapBuffers(pWindow);
	}
	

	glfwTerminate();

	return 0;
}


void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}


void showFPS(GLFWwindow * window)
{
	static double previousSeconds = 0.0;
	
	static int frameCount = 0;
	
	double elapsedSeconds;
	
	// return number of seconds since GLFW started, as a double
	double currentSeconds = glfwGetTime();

	elapsedSeconds = currentSeconds - previousSeconds;

	// limit text update 4 times per second
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		
		double fps = (double)frameCount / elapsedSeconds;

		double msPerFrame = 1000.0 / fps;

		// create a text
		std::ostringstream outs;

		// 3 digits precision
		outs.precision(3);

		outs << std::fixed
			<< APP_TITLE
			<< "  "
			<< "FPS: " 
			<< fps 
			<< "  "
			<< "Frame Time: " 
			<< msPerFrame 
			<< " (ms) ";
	
		glfwSetWindowTitle(window, outs.str().c_str());
	
		frameCount = 0;
	}

	frameCount++;
}
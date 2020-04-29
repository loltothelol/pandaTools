#include <pandaGLFW/window.hpp>

#include <atomic>
// #include <chrono>
// #include <iostream>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*
	Static variables used for window status tracking
*/

static thread_local GLFWwindow *g_handle = nullptr;
static std::atomic_bool g_glad_loaded { false };

/*
	Purpose: Create and setup the native window used for the Window class
*/

static void createNativeWindow(int width, int height, const char *title)
{
	/* initialise glfw (if we haven't already) */
	glfwInit();

	/* setup the native window hints */	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	/* create the native window */
	g_handle = glfwCreateWindow(width, height, title, NULL, NULL);

	/* if native window creation failed */
	if (!g_handle) throw std::runtime_error("Error creating native window!");
}

static void setupContext()
{
	/* make the newly created window's context current */
	glfwMakeContextCurrent(g_handle);

	/* enable v-sync */
	glfwSwapInterval(1);

	/* load all OpenGL functions */
	if (!g_glad_loaded)
	{
		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
			throw std::runtime_error("Failed to load OpenGL functions!");

		g_glad_loaded = true;
	}
}

void Window::init(int width, int height, const char *title)
{
	createNativeWindow(width, height, title);
	setupContext();

	Event::setupCallbacks();
	Input::setupCallbacks();
}

void Window::destroy() noexcept
{
	glfwDestroyWindow(g_handle);
}

bool Window::isInit() noexcept
{
	return g_handle;
}

void Window::checkInit()
{
	if (!isInit()) 
		throw std::logic_error("Window has not been initialised!");
}

GLFWwindow *Window::getHandle()
{
	return g_handle;
}
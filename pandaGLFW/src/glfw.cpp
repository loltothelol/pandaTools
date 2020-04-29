#include <pandaGLFW/glfw.hpp>

#include <stdexcept>

#include <GLFW/glfw3.h>

void GLFW::init()
{
	if (!glfwInit()) throw std::runtime_error("Error initialising GLFW!");
}

double GLFW::getTime()
{
	return glfwGetTime();
}

void GLFW::setTime(double time)
{
	glfwSetTime(time);
}
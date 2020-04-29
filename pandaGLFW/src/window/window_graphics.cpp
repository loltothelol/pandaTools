#include <pandaGLFW/window.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

glm::ivec2 Window::Graphics::getSize()
{
	glm::ivec2 size;
	glfwGetWindowSize(getHandle(), &size.x, &size.y);
	return size;
}

glm::vec2 Window::Graphics::getScale()
{
	glm::vec2 scale;
	glfwGetWindowContentScale(getHandle(), &scale.x, &scale.y);
	return scale;
}

void Window::Graphics::setViewport(int x, int y, int width, int height)
{
	Window::checkInit();
	glViewport(x, y, width, height);
	Event::setDirty();
}

void Window::Graphics::setClearColor(float red, float green, float blue, float alpha)
{
	Window::checkInit();
	glClearColor(red, green, blue, alpha);
}

void Window::Graphics::clear()
{
	Window::checkInit();
	glClear(GL_COLOR_BUFFER_BIT);
	Event::setDirty();
}
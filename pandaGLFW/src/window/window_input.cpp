#include <pandaGLFW/window.hpp>

#include <GLFW/glfw3.h>

using namespace Window;

static thread_local Input::Listener * g_listener = nullptr;

/*
	Native GLFW event callbacks used for the InputListener class
*/

static void onKeyEventCallback(GLFWwindow *native, int key, int scancode, int action, int mods)
{
	if (!g_listener) return;
	g_listener->onKeyEvent(static_cast<GLFW::Key>(key), static_cast<GLFW::Action>(action), static_cast<GLFW::Modifier>(mods));
}

static void onMouseButtonEventCallback(GLFWwindow *native, int button, int action, int mods)
{
	if (!g_listener) return;
	g_listener->onMouseButtonEvent(static_cast<GLFW::MouseButton>(button), static_cast<GLFW::Action>(action), static_cast<GLFW::Modifier>(mods));
}

/*
	Input function implementations
*/

void Input::setupCallbacks()
{
	GLFWwindow *handle = getHandle();
	glfwSetKeyCallback(handle, onKeyEventCallback);
	glfwSetMouseButtonCallback(handle, onMouseButtonEventCallback);
}

void Input::setListener(Listener * const & listener)
{
	g_listener = listener;
}

glm::vec2 Input::getCursorPos()
{
	double x, y;
	glfwGetCursorPos(getHandle(), &x, &y);
	return { x, y };
}

bool Input::isKeyPressed(GLFW::Key key)
{
	return glfwGetKey(getHandle(), static_cast<int>(key)) == GLFW_PRESS;
}
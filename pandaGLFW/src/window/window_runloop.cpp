#include <pandaGLFW/window.hpp>

#include <GLFW/glfw3.h>

static thread_local bool g_mainLoopRunning = false;

void Window::RunLoop::start(bool constantRedraw)
{
	GLFWwindow *handle = getHandle();
	g_mainLoopRunning = true;
	while (!glfwWindowShouldClose(handle))
	{
		// std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		/* Swap front and back buffers */
		if (constantRedraw || Event::isDirty())
		{
			Window::Event::dispatchWindowRedraw();
			glfwSwapBuffers(handle);
			Event::setDirty(false);
		}

		/* Wait for and process events */
		if (constantRedraw) glfwPollEvents();
		else glfwWaitEvents();

		// std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		// std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
	}

	g_mainLoopRunning = false;
}

void Window::RunLoop::interrupt()
{
	if (g_mainLoopRunning) glfwPostEmptyEvent();
}
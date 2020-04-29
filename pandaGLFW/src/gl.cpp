#include <pandaGLFW/gl.hpp>

#include <glad/glad.h>

#include <pandaGLFW/window.hpp>

void GL::enableScissorTest(bool scissorTest)
{
	Window::checkInit();
	if (scissorTest) glEnable(GL_SCISSOR_TEST);
	else glDisable(GL_SCISSOR_TEST);
}

void GL::setScissorBox(const glm::ivec4 &scissorBox)
{
	Window::checkInit();
	glScissor(scissorBox.x, scissorBox.y, scissorBox.z, scissorBox.w);
}
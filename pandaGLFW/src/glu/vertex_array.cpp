#include <pandaGLFW/glu/vertex_array.hpp>

#include <stdexcept>
#include <string>

#include <glad/glad.h>

#include <pandaGLFW/gl.hpp>
#include <pandaGLFW/glu/vertex_buffer.hpp>
#include <pandaGLFW/window.hpp>

using namespace GLU;

static GLuint createVertexArray()
{
	Window::checkInit();

	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	
	return vertexArray;
}

VertexArray::VertexArray() :
		m_handle(createVertexArray())
{

}

static GLenum getAttribTypeId(AttribType type)
{
	switch (type)
	{
		case AttribType::FLOAT: return GL_FLOAT;
		default: throw std::invalid_argument("");
	}
}

static std::size_t getAttribTypeSize(AttribType type)
{
	switch (type)
	{
		case AttribType::FLOAT: return sizeof(GLfloat);
		default: throw std::invalid_argument("");
	}
}

void VertexArray::registerBuffer(const VertexBuffer &buffer, int attribIndex, int size, AttribType type, int stride, int offset)
{
	Window::checkInit();

	glBindVertexArray(m_handle);
	glBindBuffer(GL_ARRAY_BUFFER, buffer.handle());

	glEnableVertexAttribArray(attribIndex);
	switch (glGetError())
	{
		case GL_NO_ERROR: break;
		case GL_INVALID_VALUE: throw std::runtime_error(std::string("Invalid attrib index: ") + std::to_string(attribIndex));
		default: throw std::runtime_error("Error enabling vertex attrib array!");
	}

	/* bind the vertex buffer to the attrib index */
	{
		GLenum attribTypeId = getAttribTypeId(type);
		std::size_t attribTypeSize = getAttribTypeSize(type);
		glVertexAttribPointer(attribIndex, size, attribTypeId, GL_FALSE, stride * attribTypeSize, reinterpret_cast<const void *>(offset * attribTypeSize));
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

static GLenum getRenderModeEnumId(GL::RenderMode mode)
{
	switch (mode)
	{
		case GL::RenderMode::TRIANGLES: return GL_TRIANGLES;
		case GL::RenderMode::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
		case GL::RenderMode::TRIANGLE_FAN: return GL_TRIANGLE_FAN;
		default: throw std::invalid_argument("");
	}
}

void VertexArray::draw(GL::RenderMode mode, int first, int count) const
{
	Window::checkInit();

	glBindVertexArray(m_handle);

	/* setup blending options */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	glDrawArrays(getRenderModeEnumId(mode), first, count);

	glBindVertexArray(0);
}

unsigned int VertexArray::handle() const
{
	return m_handle;
}
#include <pandaGLFW/glu/vertex_buffer.hpp>

#include <glad/glad.h>

#include <pandaGLFW/window.hpp>

using namespace GLU;

VertexBuffer::VertexBuffer()
{	
	Window::checkInit();
	glGenBuffers(1, &m_handle);
}

void VertexBuffer::upload(const void *data, int size)
{
	Window::checkInit();
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	m_size = size;
}

unsigned int VertexBuffer::handle() const
{
	return m_handle;
}
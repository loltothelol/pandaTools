#include <pandaCanvas/surface.hpp>

#include <stdio.h>
#include <string.h>

#include <glm/vec4.hpp>

Surface::Surface(unsigned int width, unsigned int height, unsigned int channels) :
		m_buffer(new unsigned char[width * height * channels]),
		m_width(width),
		m_height(height),
		m_channels(channels)
{

}

Surface::Surface(const unsigned char *buffer, unsigned int width, unsigned int height, unsigned int channels) :
		Surface(width, height, channels)
{
	memcpy(m_buffer, buffer, width * height * channels);
}

glm::ivec4 Surface::fetchPixel(unsigned int x, unsigned int y) const
{
	int offset = (y * m_width * m_channels) + (x * m_channels);
	glm::ivec4 val { 0, 0, 0, 255 };
	for (unsigned int i = 0; i < m_channels; i++)
	{
		val[i] = m_buffer[offset + i];
	}

	return val;
}

void Surface::setPixel(unsigned int x, unsigned int y, const glm::ivec4 &val) const
{
	unsigned long offset = (y * m_width * m_channels) + (x * m_channels);
	for (unsigned int i = 0; i < m_channels; i++)
	{
		m_buffer[offset + i] = val[i];
	}
}

unsigned char *Surface::buffer() const
{
	return m_buffer;
}

unsigned int Surface::width() const
{
	return m_width;
}

unsigned int Surface::height() const
{
	return m_height;
}

unsigned int Surface::channels() const
{
	return m_channels;
}

// Surface Surface::convertTo(unsigned int channels, glm::ivec4 placeholder)
// {
// 	Surface surface { m_width, m_height, channels };
// 	for (int y = 0; y < m_height; y++)
// 	{
// 		for (int x = 0; x < m_width; x++)
// 		{
// 			glm::ivec4 val { placeholder };
// 			fetchPixel(x, y, val);
// 			surface.setPixel(x, y, val);
// 		}
// 	}

// 	return surface;
// }
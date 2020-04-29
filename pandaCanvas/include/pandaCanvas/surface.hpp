#ifndef PANDACANVAS_SURFACE_H_
#define PANDACANVAS_SURFACE_H_

#include <glm/vec4.hpp>

class Surface
{
public:
	Surface(unsigned int width, unsigned int height, unsigned int channels);
	Surface(const unsigned char *buffer, unsigned int width, unsigned int height, unsigned int channels);

	Surface(const Surface&) = delete;
	Surface(Surface&&) = default;

	glm::ivec4 fetchPixel(unsigned int x, unsigned int y) const;
	void setPixel(unsigned int x, unsigned int y, const glm::ivec4 &val) const;

	// Surface convertTo(unsigned int channels, glm::ivec4 placeholder);

	Surface &operator=(const Surface&) = delete;
	Surface &operator=(Surface&&) = default;

	unsigned char *buffer() const;
	unsigned int width() const;
	unsigned int height() const;
	unsigned int channels() const;

private:
	unsigned char *m_buffer;
	unsigned int m_width, m_height, m_channels;
};

#endif
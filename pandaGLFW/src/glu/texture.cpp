#include <pandaGLFW/glu/texture.hpp>

#include <stdexcept>
#include <string>

#include <glad/glad.h>
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

#include <pandaGLFW/window.hpp>

using namespace GLU;

//Texture Texture::loadFromFile(const std::string &file)
//{
//	/* load the image */
//	int width, height;
//	unsigned char *data = stbi_load(file.c_str(), &width, &height, NULL, 4);
//	if (data == NULL) throw std::runtime_error("Error reading pixel data from image!");
//
//	/* create the texture and upload the pixels */
//	Texture texture {};
//	texture.upload(data, width, height, 4);
//
//	return texture;
//}

Texture::Texture()
{
	Window::checkInit();
	glGenTextures(1, &m_handle);
}

static GLenum getFormatForChannels(int channels)
{
	switch (channels)
	{
		case 1: return GL_RED;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default: throw std::runtime_error("No format found for channels!");
	}
}

void Texture::upload(const unsigned char *data, int width, int height, int channels)
{
	Window::checkInit();

	glBindTexture(GL_TEXTURE_2D, m_handle);
	
	/* upload the texture data */
	GLenum format = getFormatForChannels(channels);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	
	/* configure the texture parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	m_width = width;
	m_height = height;
}

void Texture::bindTo(int unit) const
{
	Window::checkInit();

	int max_units;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_units);

	if (unit >= max_units)
		throw std::runtime_error("Texture unit is out-of-bounds!");
	
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::destroy()
{
	Window::checkInit();
	glDeleteTextures(1, &m_handle);
}

unsigned int Texture::handle() const
{
	return m_handle;
}

int Texture::width() const
{
	return m_width;
}

int Texture::height() const
{
	return m_height;
}

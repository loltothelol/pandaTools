#ifndef GL_TEXTURE_H_
#define GL_TEXTURE_H_

#include <memory>
#include <string>

namespace GLU
{
	class Texture
	{
	public:
		//static void bind(unsigned int texture, unsigned int unit);
		//static Texture loadFromFile(const std::string &file);

		Texture();
		Texture(const Texture&) = delete;
		Texture(Texture&&) = default;

		void upload(const unsigned char *data, int width, int height, int channels);
		void bindTo(int unit) const;

		void destroy();

		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&) = default;

		int width() const;
		int height() const;
		unsigned int handle() const;

	private:
		unsigned int m_handle = 0;
		int m_width = -1, m_height = -1;
	};
}

#endif
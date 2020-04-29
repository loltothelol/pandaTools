#ifndef GL_VERTEX_BUFFER_H_
#define GL_VERTEX_BUFFER_H_

#include <vector>
#include <memory>

namespace GLU
{
	class VertexBuffer
	{
	public:
		VertexBuffer();

		void upload(const void *data, int size);

		template<class T>
		void upload(const std::vector<T> &data, int count)
		{
			upload(&data[0], sizeof(T) * count);
		}

		template<class T>
		void upload(std::initializer_list<T> list)
		{
			std::vector<T> data (list.size());
			data.insert(data.begin(), list.begin(), list.end());
			upload(data, data.size());
		}

		unsigned int handle() const;

	private:
		unsigned int m_handle, m_size;
	};
}

#endif
#ifndef VERTEX_ARRAY_H_
#define VERTEX_ARRAY_H_

#include <memory>

#include "../gl.hpp"

namespace GLU
{
	class VertexBuffer;
	
	enum class AttribType
	{
		FLOAT
	};

	class VertexArray
	{
	public:
		VertexArray();

		void registerBuffer(const VertexBuffer &buffer, int attribIndex, int size, AttribType type, int stride = 0, int offset = 0);
		void draw(GL::RenderMode mode, int first, int count) const;

		unsigned int handle() const;
		
	private:
		unsigned int m_handle;
	};

}

#endif
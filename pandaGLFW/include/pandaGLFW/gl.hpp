#ifndef PANDAGL_CONTEXT_H_
#define PANDAGL_CONTEXT_H_

#include <glm/vec4.hpp>

namespace GL
{
	enum class RenderMode
	{
		TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN
	};

	void enableScissorTest(bool scissorTest);
	void setScissorBox(const glm::ivec4 &scissorBox);
};

#endif
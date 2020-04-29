#ifndef CANVAS_HOST_ENGINE_H_
#define CANVAS_HOST_ENGINE_H_

#include <memory>
#include <vector>

#include <glm/mat4x4.hpp>

namespace GL
{
	enum class RenderMode;
}

namespace GLU
{
	class Program;
	class Texture;
	class Uniform;
	class VertexArray;
	class VertexBuffer;
}

class HostEngine
{
public:
	HostEngine();
	~HostEngine();

	void upload(const std::vector<float> &vertexData);
	void draw(int first, int count, GL::RenderMode mode, const glm::mat4 &matrix, const GLU::Texture *texture, const glm::vec4 *scissorBox);

private:
	std::unique_ptr<GLU::Program> m_program;
	std::unique_ptr<GLU::Uniform> m_matrixUniform;
	std::unique_ptr<GLU::Uniform> m_samplerUniform;
	std::unique_ptr<GLU::Uniform> m_useTextureUniform;

	std::unique_ptr<GLU::VertexArray> m_vertexArray;
	std::unique_ptr<GLU::VertexBuffer> m_vertexBuffer;
};

#endif
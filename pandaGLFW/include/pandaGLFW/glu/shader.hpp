#ifndef PANDAGL_SHADER_H_
#define PANDAGL_SHADER_H_

namespace GLU
{
	enum class ShaderType
	{
		VERTEX, FRAGMENT
	};

	class Shader
	{
	public:
		Shader(ShaderType type, const std::string& src);
		Shader(const Shader&) = delete;
		Shader(Shader&&) = default;
		~Shader();

		Shader& operator=(const Shader&) = delete;
	  	Shader& operator=(Shader&&) = default;

	  	unsigned int handle() const;

	private:
		unsigned int m_handle;
	};
}

#endif
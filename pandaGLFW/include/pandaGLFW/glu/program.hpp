#ifndef GL_PROGRAM_H_
#define GL_PROGRAM_H_

#include <memory>
#include <string>

namespace GLU
{
	class Shader;

	class Program
	{
	public:
		static void setActive(const Program &program);

		Program();
		Program(const Program&) = delete;
		Program(Program&&) = default;

		class Uniform getUniform(const std::string &name);
		int getAttribIndex(const std::string &name) const;

		void attachShader(const Shader &shader);
		void link();

		Program& operator=(const Program&) = delete;
	  	Program& operator=(Program&&) = default;

	private:
		unsigned int m_handle;

		friend void setActiveProgram(const Program &program);
	};

	class Uniform
	{
	public:
		Uniform(unsigned int program, int location);

		template<class T>
		void set(const T& val);

	private:
		unsigned int m_program;
		int m_location;
	};
};

#endif
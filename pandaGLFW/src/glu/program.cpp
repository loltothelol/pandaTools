#include <pandaGLFW/glu/program.hpp>

#include <fstream>
#include <memory>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include <pandaGLFW/glu/shader.hpp>
#include <pandaGLFW/glu/vertex_array.hpp>
#include <pandaGLFW/window.hpp>

static GLenum getShaderTypeId(GLU::ShaderType type)
{
	using ST = GLU::ShaderType;
	switch (type)
	{
		case ST::VERTEX: return GL_VERTEX_SHADER;
		case ST::FRAGMENT: return GL_FRAGMENT_SHADER;
	}
}

GLU::Shader::Shader(ShaderType type, const std::string& src)
{
	Window::checkInit();

	m_handle = glCreateShader(getShaderTypeId(type));
	if (m_handle == 0) throw std::runtime_error("Error creating shader!");

	/* upload the shader source */
	const char *c_str = src.c_str();
	glShaderSource(m_handle, 1, &c_str, NULL);
	
	/* compile the shader */
	glCompileShader(m_handle);

	int compileStatus;
	glGetShaderiv(m_handle, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		int infoLogLength;
		glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &infoLogLength);
		
		char *infoLog = new char[infoLogLength];
		glGetShaderInfoLog(m_handle, infoLogLength, NULL, infoLog);
		throw std::runtime_error(std::string("Error compiling shader: ") + std::string(infoLog));
	}
}

GLU::Shader::~Shader()
{
	
}

unsigned int GLU::Shader::handle() const
{
	return m_handle;
}

/*****************************************************************************/

void GLU::Program::setActive(const Program &program)
{
	Window::checkInit();
	glUseProgram(program.m_handle);
}

/*===========================================================================*/

static GLuint createProgram()
{
	Window::checkInit();

	GLuint program = glCreateProgram();
	if (program == 0) throw std::runtime_error("Error creating program!");

	return program;
}

GLU::Program::Program() :
		m_handle(createProgram())
{

}

auto GLU::Program::getUniform(const std::string &name) -> Uniform
{	
	Window::checkInit();

	int location = glGetUniformLocation(m_handle, name.c_str());
	if (location == -1) throw std::runtime_error("Invalid uniform name!");
	return Uniform { m_handle, location };
}

int GLU::Program::getAttribIndex(const std::string &name) const
{
	Window::checkInit();
	return glGetAttribLocation(m_handle, name.c_str());
}

void GLU::Program::attachShader(const Shader &shader)
try
{
	Window::checkInit();
	glAttachShader(m_handle, shader.handle());
}
catch (std::logic_error &e)
{
	throw std::logic_error(std::string("Error attatching shader: ") + e.what());
}

void GLU::Program::link()
{
	Window::checkInit();

	/* link the program */
	glLinkProgram(m_handle);

	int linkStatus;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE)
	{
		int infoLogLength;
		glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &infoLogLength);
		
		char *infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_handle, infoLogLength, NULL, infoLog);
		throw std::runtime_error(std::string("Error linking program: ") + std::string(infoLog));
	}
}

/*===========================================================================*/

GLU::Uniform::Uniform(unsigned int program, int location) :
		m_program(program),
		m_location(location)
{

}

template<>
void GLU::Uniform::set<int>(const int& val)
{
	Window::checkInit();

	glUseProgram(m_program);
	glUniform1i(m_location, val);
}

template<>
void GLU::Uniform::set<bool>(const bool& val)
{
	Window::checkInit();
	set<int>(val);
}

template<>
void GLU::Uniform::set<glm::mat4>(const glm::mat4& val)
{
	Window::checkInit();

	glUseProgram(m_program);
	glUniformMatrix4fv(m_location, 1, GL_FALSE, &val[0][0]);
}

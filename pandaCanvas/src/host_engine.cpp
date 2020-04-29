#include "host_engine.h"

#include <stdexcept>
#include <string>

#include <pandaGLFW/glu/glu.hpp>

static const std::string vertexShaderSrc =
"#version 330\n"

"uniform mat4 matrix;\n"

"in vec4 in_pos;\n"
"in vec4 in_color;\n"

"out vec2 ex_texCoords;\n"
"out vec4 ex_color;\n"

"void main(){\n"
"gl_Position=matrix*vec4(in_pos.xy,0.0,1.0);\n"
"ex_texCoords=in_pos.zw;\n"
"ex_color=in_color;\n"
"}";

static const std::string fragmentShaderSrc =
"#version 330\n"

"uniform sampler2D sampler;\n"
"uniform bool useTexture;\n"

"in vec2 ex_texCoords;\n"
"in vec4 ex_color;\n"
"out vec4 fragColor;\n"

"void main(){\n"
"if(useTexture){\n"
// "vec4 sV=texture(sampler, ex_texCoords);\n"
// "sV.xyz+=(ex_color.xyz-sV.xyz)*ex_color.w;\n"
// "fragColor=sV;\n"
"fragColor=texture(sampler, ex_texCoords)*ex_color;\n"
"}else{\n"
"fragColor=ex_color;\n"
"}\n"
"}";

static GLU::Program createProgram()
try
{
	GLU::Program program { };

	GLU::Shader vertShader { GLU::ShaderType::VERTEX, vertexShaderSrc };
	GLU::Shader fragShader { GLU::ShaderType::FRAGMENT, fragmentShaderSrc };

	program.attachShader( vertShader );
	program.attachShader( fragShader );
	program.link();

	return program;
}
catch ( std::exception & e )
{
	throw std::runtime_error( std::string( "Error creating program: " ) + e.what() );
}

HostEngine::HostEngine()
try :
	m_program( new GLU::Program( createProgram() ) ),
	m_matrixUniform( new GLU::Uniform( m_program->getUniform( "matrix" ) ) ),
	m_samplerUniform( new GLU::Uniform( m_program->getUniform( "sampler" ) ) ),
	m_useTextureUniform( new GLU::Uniform( m_program->getUniform( "useTexture" ) ) ),

	m_vertexArray( new GLU::VertexArray() ),
	m_vertexBuffer( new GLU::VertexBuffer() )
{
	/* setup the vertex array */
	m_vertexArray->registerBuffer( *m_vertexBuffer, m_program->getAttribIndex( "in_pos" ), 4, GLU::AttribType::FLOAT, 8, 0 );
	m_vertexArray->registerBuffer( *m_vertexBuffer, m_program->getAttribIndex( "in_color" ), 4, GLU::AttribType::FLOAT, 8, 4 );
}
catch ( std::exception & e )
{
	throw std::runtime_error( std::string( "Error creating HostEngine: " ) + e.what() );
}

HostEngine::~HostEngine()
{

}

void HostEngine::upload( const std::vector<float> & vertexData )
{
	if ( vertexData.size() % 8 != 0 )
		throw std::runtime_error( "Vertex data buffer size is not a multiple of 8!" );

	m_vertexBuffer->upload( vertexData, static_cast<int>( vertexData.size() ) );
}

void HostEngine::draw( int first, int count, GL::RenderMode mode, const glm::mat4 & matrix, const GLU::Texture * texture, const glm::vec4 * scissorBox )
{
	m_matrixUniform->set( matrix );
	m_samplerUniform->set( 0 );

	m_useTextureUniform->set<int>( texture != nullptr );
	if ( texture ) texture->bindTo( 0 );

	GL::enableScissorTest( scissorBox != nullptr );
	if ( scissorBox ) GL::setScissorBox( *scissorBox );

	GLU::Program::setActive( *m_program );
	m_vertexArray->draw( mode, first, count );

	if ( scissorBox ) GL::enableScissorTest( false );
}
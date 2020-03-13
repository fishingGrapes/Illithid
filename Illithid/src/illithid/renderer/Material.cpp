#include "ilpch.h"
#include "Material.h"

#include "illithid/core/Log.h"
#include "glad/glad.h"

namespace itd
{
	Material::Material( const Shader& shader )
	{
		this->CreateProgram( shader );
	}

	Material::~Material( )
	{
		glDeleteProgram( program_ );
	}

	void Material::CreateProgram( const Shader& shader )
	{


		const char* vertexSource = shader.VertexSource.c_str( );
		const char* fragmentSource = shader.FragmentSource.c_str( );

		uint32_t vertexShader, fragmentShader;

		vertexShader = glCreateShader( GL_VERTEX_SHADER );
		glShaderSource( vertexShader, 1, &vertexSource, nullptr );
		glCompileShader( vertexShader );


		fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource( fragmentShader, 1, &fragmentSource, nullptr );
		glCompileShader( fragmentShader );

		program_ = glCreateProgram( );
		glAttachShader( program_, vertexShader );
		glAttachShader( program_, fragmentShader );
		glLinkProgram( program_ );


#if defined(IL_DEBUG) || defined(IL_RELEASE)

		int32_t success;
		char vInfoLog[ 512 ], fInfoLog[ 512 ], pInfoLog[ 512 ];

		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &success );
		if (!success)
		{
			glGetShaderInfoLog( vertexShader, 512, nullptr, vInfoLog );
			IL_CORE_ERROR( "Error when compiling vertex shader : {0}", vInfoLog );
		}

		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &success );
		if (!success)
		{
			glGetShaderInfoLog( fragmentShader, 512, nullptr, fInfoLog );
			IL_CORE_ERROR( "Error when compiling fragment shader : {0}", fInfoLog );
		}

		glGetProgramiv( program_, GL_LINK_STATUS, &success );
		if (!success)
		{
			glGetProgramInfoLog( program_, 512, nullptr, pInfoLog );
			IL_CORE_ERROR( "Error when linking shader program : {0}", pInfoLog );
		}

#endif

		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
	}

	void Material::Use( )
	{
		glUseProgram( program_ );
	}
}
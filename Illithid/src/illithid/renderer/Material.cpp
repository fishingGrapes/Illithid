#include "ilpch.h"
#include "Material.h"

#include "illithid/core/Log.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Texture2D.h"

namespace itd
{
	Material::Material( const Shader& shader )
	{
		this->CreateProgram( shader );
		this->CacheUniforms( );
	}

	Material::~Material( )
	{
		glDeleteProgram( program_ );
	}

	void Material::CreateProgram( const Shader& shader )
	{
		const char* vertexSource = shader.VertexSource.c_str( );
		const char* fragmentSource = shader.FragmentSource.c_str( );

		GLuint vertexShader, fragmentShader;

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

		GLint success;
		GLchar vInfoLog[ 512 ], fInfoLog[ 512 ], pInfoLog[ 512 ];

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

	//TODO: Parse sampler2D and assign the respective textures
	void Material::CacheUniforms( )
	{
		GLint uniformCount = 0;
		glGetProgramiv( program_, GL_ACTIVE_UNIFORMS, &uniformCount );

		if (uniformCount > 0)
		{
			GLint maxNameLength = 0;
			GLsizei length = 0;
			GLsizei size = 0;
			GLenum type = GL_NONE;

			glGetProgramiv( program_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength );
			GLchar* uniformName = new GLchar[ maxNameLength ];

			for (GLint i = 0; i < uniformCount; ++i)
			{
				glGetActiveUniform( program_, i, maxNameLength, &length, &size, &type, uniformName );

				UniformData data;
				data.Location = glGetUniformLocation( program_, uniformName );
				data.Size = size;

				uniformMap_.emplace( std::make_pair( std::string( uniformName, length ), data ) );
			}

			delete[ ] uniformName;
		}
	}

	//TODO: implement code to re-assign textures for the same uniform.
	void Material::SetTexture( const std::string& uniform, std::shared_ptr<Texture2D> texture )
	{
		if (textureMap_.find( uniform ) == textureMap_.end( ))
		{
			int32_t unit = static_cast<int32_t>( textureMap_.size( ) );
			textureMap_[ uniform ] = TextureData{ texture , unit };

			UniformData& data = uniformMap_[ uniform ];
			glProgramUniform1i( program_, data.Location, unit );

			return;
		}

		TextureData& data = textureMap_[ uniform ];
		data.Texture = texture;
	}

	void Material::Use( )
	{
		uint32_t textureUnit = 0;
		for (auto& element : textureMap_)
		{
			element.second.Texture->BindToTextureUnit( textureUnit++ );
		}

		glUseProgram( program_ );
	}

	void Material::SetInt( const char* uniform, int32_t value )
	{
		UniformData& data = uniformMap_[ uniform ];
		glProgramUniform1i( program_, data.Location, value );
	}

	void Material::SetFloat( const char* uniform, float_t value )
	{
		UniformData& data = uniformMap_[ uniform ];
		glProgramUniform1f( program_, data.Location, value );
	}

	void Material::SetVector2f( const char* uniform, glm::vec2& value )
	{
		UniformData& data = uniformMap_[ uniform ];
		glProgramUniform2fv( program_, data.Location, 1, glm::value_ptr( value ) );
	}
}
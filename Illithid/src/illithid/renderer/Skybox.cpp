#include "ilpch.h"
#include "Skybox.h"

#include "illithid/core/FileSystem.h"
#include "illithid/core/Log.h"
#include "illithid/core/Resources.h"
#include "illithid/components/Camera.h"

#include "nlohmann/json.hpp"
#include "glad/glad.h"
#include "stb_image.h"

namespace itd
{
	Skybox::Skybox( const std::string& file ) :id_( 0 )
	{
		nlohmann::json& skybox = nlohmann::json::parse( FileSystem::ReadAsText( file ) );
		std::vector<std::string> texture_paths = skybox;

		LoadCubemap( texture_paths );

		Shader skyboxShader( "Assets/Shaders/skybox.shader" );
		material_ = std::make_unique<Material>( skyboxShader );

		box_ = Resources::CubeMesh;
	}

	Skybox::~Skybox( )
	{
		glDeleteTextures( 1, &id_ );
	}

	void Skybox::Render( )
	{
		glCullFace( GL_FRONT );
		glDepthFunc( GL_LEQUAL );

		material_->Use( );
		glBindTextureUnit( 0, id_ );
		material_->SetInt( "u_SkyBox", 0 );

		dptr<Camera> camera = Camera::Primary( );
		material_->SetMatrix4f( "u_View", glm::mat4( glm::mat3( camera->ViewMatrix( ) ) ) );
		material_->SetMatrix4f( "u_Projection", camera->ProjectionMatrix( ) );

		box_->Bind( );
		glDrawArrays( GL_TRIANGLES, 0, static_cast<GLsizei>( box_->VertexCount( ) ) );

		glDepthFunc( GL_LESS );
		glCullFace( GL_BACK );
	}

	void Skybox::LoadCubemap( const std::vector<std::string>& faces )
	{
		stbi_set_flip_vertically_on_load( false );

		glGenTextures( 1, &id_ );
		glBindTexture( GL_TEXTURE_CUBE_MAP, id_ );

		int32_t width, height, channels;
		for (size_t i = 0; i < faces.size( ); ++i)
		{
			unsigned char* data = stbi_load( faces[ i ].c_str( ), &width, &height, &channels, STBI_rgb );
			if (data)
			{
				glTexImage2D( static_cast<GLenum>( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i ), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
				//glGenerateMipmap( static_cast<GLenum>( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i ) );
			}
			else
			{
				IL_CORE_ERROR( "Failed to texture at path: {0}", faces[ i ] );
			}
			stbi_image_free( data );
		}

		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

	}

}
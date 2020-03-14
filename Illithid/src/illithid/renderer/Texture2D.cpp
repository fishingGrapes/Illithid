#include "ilpch.h"
#include "Texture2D.h"

#include "illithid/core/FileSystem.h"
#include "illithid/core/Log.h"
#include "glm/glm.hpp"

#include "glad/glad.h"

#include "stb_image.h"

namespace itd
{

	//TODO: Change this to take the 5th parameter in the stbi_load function
	std::unique_ptr<Texture2D> Texture2D::Load( const std::string& path )
	{
		std::unique_ptr<Texture2D> texture = std::make_unique<Texture2D>( );

		stbi_set_flip_vertically_on_load( true );
		unsigned char* data = stbi_load( FileSystem::GetAbsolutePath( path ).c_str( ), &texture->width_, &texture->height_, &texture->channels_, 0 );

		if (data)
		{
			glCreateTextures( GL_TEXTURE_2D, 1, &texture->id_ );

			glTextureParameteri( texture->id_, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTextureParameteri( texture->id_, GL_TEXTURE_WRAP_T, GL_REPEAT );
			glTextureParameteri( texture->id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			glTextureParameteri( texture->id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

			glTextureStorage2D( texture->id_, CalculateMipmapLevels( texture->width_, texture->height_ ), GL_RGB8, texture->width_, texture->height_ );
			glTextureSubImage2D( texture->id_, 0, 0, 0, texture->width_, texture->height_, GL_RGB, GL_UNSIGNED_BYTE, data );
			glGenerateTextureMipmap( texture->id_ );
		}
		else
		{
			IL_CORE_ERROR( "Failed to load texture : {0}", path );
		}

		stbi_image_free( data );
		return std::move( texture );
	}

	int32_t Texture2D::CalculateMipmapLevels( int32_t width, int32_t height )
	{
		return static_cast<GLint>( glm::floor( glm::log2( static_cast<float_t>( glm::max( width, height ) ) ) ) );
	}


	void Texture2D::Bind( ) const
	{
		glBindTextureUnit( 0, id_ );
	}
}

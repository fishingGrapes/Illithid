#include "ilpch.h"
#include "Texture2D.h"

#include "illithid/core/FileSystem.h"
#include "illithid/core/Log.h"

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "stb_image.h"

#include "nlohmann/json.hpp"

#pragma warning( push )
#pragma warning (disable : 26812 )

namespace itd
{

	Texture2D::Texture2D( int32_t width, int32_t height, TextureFormat format = TextureFormat::TF_RGB8 )
		:id_( 0 ), width_( width ), height_( height ), pixelFormat_( -1 )
	{
		glCreateTextures( GL_TEXTURE_2D, 1, &id_ );

		SetPixelFormat( PixelFormat::PF_RGB );
		SetWrapMode_S( TextureWrapMode::WM_REPEAT );
		SetWrapMode_T( TextureWrapMode::WM_REPEAT );
		SetMinificationFilter( TextureFilterMode::FM_LINEAR_MIPMAP_LINEAR );
		SetMagnificationFilter( TextureFilterMode::FM_LINEAR );

		glTextureStorage2D( id_, CalculateMipmapLevels( width, height ), format, width, height );
	}

	Texture2D::~Texture2D( )
	{
		glDeleteTextures( 1, &id_ );
	}

	void Texture2D::SetWrapMode_S( TextureWrapMode wrapMode )
	{
		glTextureParameteri( id_, GL_TEXTURE_WRAP_S, wrapMode );
	}

	void Texture2D::SetWrapMode_T( TextureWrapMode wrapMode )
	{
		glTextureParameteri( id_, GL_TEXTURE_WRAP_T, wrapMode );
	}

	void Texture2D::SetMinificationFilter( TextureFilterMode filterMode )
	{
		glTextureParameteri( id_, GL_TEXTURE_MIN_FILTER, filterMode );
	}

	void Texture2D::SetMagnificationFilter( TextureFilterMode filterMode )
	{
		glTextureParameteri( id_, GL_TEXTURE_MAG_FILTER, filterMode );
	}

	void Texture2D::GenerateMipMaps( ) const
	{
		glGenerateTextureMipmap( id_ );
	}

	void Texture2D::SetPixelFormat( PixelFormat format )
	{
		pixelFormat_ = format;
	}

	void Texture2D::SetPixelData( const unsigned char* data )
	{
		glTextureSubImage2D( id_, 0, 0, 0, width_, height_, pixelFormat_, GL_UNSIGNED_BYTE, data );
	}

	void Texture2D::BindToTextureUnit( uint32_t unit ) const
	{
		glBindTextureUnit( unit, id_ );
	}

	std::shared_ptr<Texture2D> Texture2D::Load( const std::string& path )
	{
		stbi_set_flip_vertically_on_load( true );

		nlohmann::json& textureObject = nlohmann::json::parse( FileSystem::ReadAsText( path ) );

		TextureWrapMode wrap_s = textureObject[ "wrap_mode_s" ].get<TextureWrapMode>( );
		TextureWrapMode wrap_t = textureObject[ "wrap_mode_t" ].get<TextureWrapMode>( );

		TextureFilterMode filter_min = textureObject[ "min_filter_mode" ].get<TextureFilterMode>( );
		TextureFilterMode filter_mag = textureObject[ "mag_filter_mode" ].get<TextureFilterMode>( );

		TextureFormat format = textureObject[ "texture_format" ].get<TextureFormat>( );

		int32_t desiredChannels = 0;
		switch (format)
		{
			case TextureFormat::TF_R8:
				{
					desiredChannels = STBI_grey;
					break;
				}

			case TextureFormat::TF_RG8:
				{
					desiredChannels = STBI_grey_alpha;
					break;
				}

			case TextureFormat::TF_RGB8:
				{
					desiredChannels = STBI_rgb;
					break;
				}

			case TextureFormat::TF_RGBA8:
				{
					desiredChannels = STBI_rgb_alpha;
					break;
				}
		}

		std::string texturePath = textureObject[ "path" ];
		int32_t width = 0, height = 0, channels = 0;
		unsigned char* data = stbi_load( texturePath.c_str( ), &width, &height, &channels, desiredChannels );

		PixelFormat pixelFormat = PixelFormat::PF_INVALID;
		switch (channels)
		{
			case 1:
				{
					pixelFormat = PixelFormat::PF_RED;
					break;
				}

			case 2:
				{
					pixelFormat = PixelFormat::PF_RG;
					break;
				}

			case 3:
				{
					pixelFormat = PixelFormat::PF_RGB;
					break;
				}

			case 4:
				{
					pixelFormat = PixelFormat::PF_RGBA;
					break;
				}
		}

		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>( width, height, format );
		texture->SetPixelFormat( pixelFormat );

		if (data)
		{
			texture->SetPixelData( data );
			texture->GenerateMipMaps( );
		}
		else
		{
			IL_CORE_ERROR( "Failed to load texture : {0}", path );
		}

		stbi_image_free( data );
		stbi_set_flip_vertically_on_load( false );
		return  texture;
	}

	int32_t Texture2D::CalculateMipmapLevels( int32_t width, int32_t height )
	{
		return static_cast<GLint>( 1 + glm::floor( glm::log2( static_cast<float_t>( glm::max( width, height ) ) ) ) );
	}

	NLOHMANN_JSON_SERIALIZE_ENUM( TextureWrapMode, {
	{WM_INVALID , nullptr },
	{WM_REPEAT, "repeat" },
	{WM_MIRRORED_REPEAT, "mirrored_repeat" },
	{WM_CLAMP_TO_EDGE, "clamp_to_edge" },
	{WM_CLAMP_TO_BORDER, "clamp_to_border" },
								  } );


	NLOHMANN_JSON_SERIALIZE_ENUM( TextureFilterMode, {
	{FM_INVALID , nullptr },
	{FM_NEAREST, "nearest" },
	{FM_LINEAR, "linear" },
	{FM_NEAREST_MIPMAP_NEAREST, "nearest_mipmap_nearest" },
	{FM_LINEAR_MIPMAP_NEAREST, "linear_mipmap_nearest" },
	{FM_NEAREST_MIPMAP_LINEAR, "nearest_mipmap_linear" },
	{FM_LINEAR_MIPMAP_LINEAR, "linear_mipmap_linear" },
								  } );

	NLOHMANN_JSON_SERIALIZE_ENUM( TextureFormat, {
	{TF_INVALID , nullptr },
	{TF_R8, "R8" },
	{TF_RG8, "RG8" },
	{TF_RGB8, "RGB8" },
	{TF_RGBA8, "RGBA8" },
								  } );

}

#pragma warning( pop )

#include "ilpch.h"
#include "Texture2D.h"

#include "illithid/core/FileSystem.h"
#include "illithid/core/Log.h"

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "stb_image.h"

#include "nlohmann/json.hpp"

namespace itd
{
	enum WrapMode : int32_t
	{
		WM_INVALID = -1,
		WM_REPEAT = GL_REPEAT,
		WM_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		WM_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		WM_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
	};

	enum FilterMode : int32_t
	{
		FM_INVALID = -1,
		FM_NEAREST = GL_NEAREST,
		FM_LINEAR = GL_LINEAR,
		FM_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		FM_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
		FM_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
		FM_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
	};

	enum TextureFormat : int32_t
	{
		TF_INVALID = -1,
		TF_RED = GL_R8,
		TF_RG = GL_RG8,
		TF_RGB = GL_RGB8,
		TF_RGBA = GL_RGBA8
	};

	NLOHMANN_JSON_SERIALIZE_ENUM( WrapMode, {
	{WM_INVALID , nullptr },
	{WM_REPEAT, "repeat" },
	{WM_MIRRORED_REPEAT, "mirrored_repeat" },
	{WM_CLAMP_TO_EDGE, "clamp_to_edge" },
	{WM_CLAMP_TO_BORDER, "clamp_to_border" },
								  } );


	NLOHMANN_JSON_SERIALIZE_ENUM( FilterMode, {
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
	{TF_RED, "grayscale" },
	{TF_RG, "grayscale_with_alpha" },
	{TF_RGB, "RGB" },
	{TF_RGBA, "RGBA" },
								  } );



	std::shared_ptr<Texture2D> Texture2D::Load( const std::string& path )
	{
		stbi_set_flip_vertically_on_load( true );

		nlohmann::json& textureObject = nlohmann::json::parse( FileSystem::LoadAsText( path ) );

#pragma warning( push )
#pragma warning (disable : 26812 )
		WrapMode wrap_s = textureObject[ "wrap_mode_s" ].get<WrapMode>( );
		WrapMode wrap_t = textureObject[ "wrap_mode_t" ].get<WrapMode>( );

		FilterMode filter_min = textureObject[ "min_filter_mode" ].get<FilterMode>( );
		FilterMode filter_mag = textureObject[ "mag_filter_mode" ].get<FilterMode>( );

		TextureFormat format = textureObject[ "texture_format" ].get<TextureFormat>( );
#pragma warning( pop )

		int32_t desiredChannels = 0;
		switch (format)
		{
			case TextureFormat::TF_RED:
				{
					desiredChannels = STBI_grey;
					break;
				}

			case TextureFormat::TF_RG:
				{
					desiredChannels = STBI_grey_alpha;
					break;
				}

			case TextureFormat::TF_RGB:
				{
					desiredChannels = STBI_rgb;
					break;
				}

			case TextureFormat::TF_RGBA:
				{
					desiredChannels = STBI_rgb_alpha;
					break;
				}
		}

		std::string texturePath = textureObject[ "path" ];
		int32_t width, height, channels;
		unsigned char* data = stbi_load( texturePath.c_str( ), &width, &height, &channels, desiredChannels );

		int32_t pixelFormat = -1;
		switch (channels)
		{
			case 1:
				{
					pixelFormat = GL_RED;
					break;
				}

			case 2:
				{
					pixelFormat = GL_RG;
					break;
				}

			case 3:
				{
					pixelFormat = GL_RGB;
					break;
				}

			case 4:
				{
					pixelFormat = GL_RGBA;
					break;
				}
		}

		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>( );
		if (data)
		{
			glCreateTextures( GL_TEXTURE_2D, 1, &texture->id_ );

			glTextureParameteri( texture->id_, GL_TEXTURE_WRAP_S, wrap_s );
			glTextureParameteri( texture->id_, GL_TEXTURE_WRAP_T, wrap_t );
			glTextureParameteri( texture->id_, GL_TEXTURE_MIN_FILTER, filter_min );
			glTextureParameteri( texture->id_, GL_TEXTURE_MAG_FILTER, filter_mag );

			glTextureStorage2D( texture->id_, CalculateMipmapLevels( width, height ), format, width, height );
			glTextureSubImage2D( texture->id_, 0, 0, 0, width, height, pixelFormat, GL_UNSIGNED_BYTE, data );
			glGenerateTextureMipmap( texture->id_ );
		}
		else
		{
			IL_CORE_ERROR( "Failed to load texture : {0}", path );
		}

		stbi_image_free( data );
		return  texture;
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

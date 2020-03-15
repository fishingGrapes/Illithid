#pragma once
#include "TextureOptions.h"

namespace itd
{

	class Texture2D
	{
	public:
		static std::shared_ptr<Texture2D> Load( const std::string& path );

		explicit Texture2D( int32_t width, int32_t height, TextureFormat  format );
		~Texture2D( );

		void SetWrapMode_S( TextureWrapMode wrapMode );
		void SetWrapMode_T( TextureWrapMode wrapMode );
		void SetMinificationFilter( TextureFilterMode filterMode );
		void SetMagnificationFilter( TextureFilterMode filterMode );

		void GenerateMipMaps( ) const;
		void SetPixelFormat( PixelFormat format );
		void SetPixelData( const unsigned char* data );

		void Bind( ) const;

	private:
		uint32_t id_;
		int32_t width_, height_;
		int32_t pixelFormat_;

		static int32_t CalculateMipmapLevels( int32_t width, int32_t height );
	};
}

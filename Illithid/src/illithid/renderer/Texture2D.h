#pragma once

namespace itd
{
	class Texture2D
	{
	public:
		static std::unique_ptr<Texture2D> Load( const std::string& path );
		void Bind() const;

	private:
		int32_t width_, height_, channels_;
		uint32_t id_;

		static int32_t CalculateMipmapLevels( int32_t width, int32_t height ) ;
	};
}

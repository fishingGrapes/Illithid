#pragma once

namespace itd
{

	class Texture2D
	{
	public:
		static std::shared_ptr<Texture2D> Load( const std::string& path );
		void Bind() const;

	private:
		uint32_t id_;

		static int32_t CalculateMipmapLevels( int32_t width, int32_t height ) ;
	};
}

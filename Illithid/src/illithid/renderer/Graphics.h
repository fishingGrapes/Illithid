#pragma once
#include <memory>

namespace itd
{
	class Window;
	class Graphics
	{
	public:
		static void Initialize( std::unique_ptr<Window>& window );
		static void Destroy( );

		static void Clear( uint32_t bits );
		static void ClearColor( float_t r, float_t g, float_t b, float_t a );
	};
}
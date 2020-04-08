#pragma once

namespace itd
{
	class Window;

	class Gui
	{
	public:

		static void Initialize( std::unique_ptr<Window>& window );
		static void Destroy( );

		static void Begin( );
		static void End( std::unique_ptr<Window>& window );
	};
}
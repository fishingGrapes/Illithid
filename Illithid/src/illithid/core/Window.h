#pragma once
#include <string>

namespace itd
{
	struct WindowProperties
	{
	public:
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		explicit WindowProperties( std::string title = "Illithid Application", uint32_t width = 1280, uint32_t height = 720 )
			: Title( title ), Width( width ), Height( height )
		{

		}
	};


	class Window
	{
	public:
		explicit Window( const WindowProperties& props ) :
			windowProps_( props ), isVsync_( true )
		{
		}

		virtual ~Window( )
		{
		}

		virtual void Update( ) = 0;

		void SetVsync( bool value )
		{
			isVsync_ = value;
		}

		inline uint32_t Width( ) const
		{
			return windowProps_.Width;
		}

		inline uint32_t Height( ) const
		{
			return windowProps_.Height;
		}

		inline bool IsVsync( ) const
		{
			return isVsync_;
		}

	protected:
		WindowProperties windowProps_;
		bool isVsync_;
	};
}
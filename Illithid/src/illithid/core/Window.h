#pragma once

#include <string>
#include<functional>

#include "illithid/events/Event.h"

struct GLFWwindow;

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
		using EventListener = std::function<void( Event& )>;

	public:
		explicit Window( const WindowProperties& props );
		~Window( );

		void PollEvents( );
		void SwapBuffers( );


		inline uint32_t Width( ) const
		{
			return windowData_.Width;
		}

		inline uint32_t Height( ) const
		{
			return windowData_.Height;
		}

		inline bool IsVsync( ) const
		{
			return isVsync_;
		}

		inline void SetVsync( bool value )
		{
			isVsync_ = value;
		}

		inline void SetEventListener( const EventListener& listener )
		{
			windowData_.Listener = listener;
		}

	private:

		struct WindowData
		{
		public:
			uint32_t Width;
			uint32_t Height;
			std::string Title;

			EventListener Listener;
		};

		WindowData windowData_;
		bool isVsync_;
		GLFWwindow* window_;

		void Initialize( );
		void Cleanup( );
	};
}
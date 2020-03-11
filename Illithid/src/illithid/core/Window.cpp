#include "ilpch.h"
#include "Window.h"

#include "Log.h"

#include "GLFW/glfw3.h"

#include "illithid/events/WindowEvents.h"
#include "illithid/events/KeyEvents.h"
#include "illithid/events/MouseEvents.h"

namespace itd
{
	Window::Window( const WindowProperties& props )
		: window_( nullptr )
	{
		windowData_.Width = props.Width;
		windowData_.Height = props.Height;
		windowData_.Title = props.Title;

		this->Initialize( );
		IL_CORE_INFO( "Window Created." );
	}

	Window::~Window( )
	{
		this->Destroy( );
		IL_CORE_INFO( "Window Destroyed." );
	}

	void Window::Initialize( )
	{
		if (!glfwInit( ))
		{
			IL_CORE_CRITICAL( "Failed to initialize GLFW." );
			glfwTerminate( );
			return;
		}

		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		window_ = glfwCreateWindow( windowData_.Width, windowData_.Height, windowData_.Title.c_str( ), nullptr, nullptr );
		if (window_ == nullptr)
		{
			IL_CORE_CRITICAL( "Failed to initialize window." );
			glfwTerminate( );
			return;
		}

		this -> SetVsync( true );

		glfwSetWindowUserPointer( window_, static_cast<void*>( &windowData_ ) );

		glfwSetWindowCloseCallback( window_, [ ] ( GLFWwindow* window )
		{
			WindowData& windowData = *( static_cast<WindowData*>( glfwGetWindowUserPointer( window ) ) );
			windowData.Listener( WindowClosedEvent( ) );
		} );

		glfwSetWindowSizeCallback( window_, [ ] ( GLFWwindow* window, int32_t width, int32_t height )
		{
			WindowData& windowData = *( static_cast<WindowData*>( glfwGetWindowUserPointer( window ) ) );
			windowData.Listener( WindowResizedEvent( width, height ) );
		} );

		glfwSetKeyCallback( window_, [ ] ( GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods )
		{
			WindowData& windowData = *( static_cast<WindowData*>( glfwGetWindowUserPointer( window ) ) );

			switch (action)
			{
				case GLFW_PRESS:
					{
						windowData.Listener( KeyPressedEvent( key, 0 ) );
						break;
					}
				case GLFW_RELEASE:
					{
						windowData.Listener( KeyReleasedEvent( key ) );
						break;
					}

				case GLFW_REPEAT:
					{
						windowData.Listener( KeyPressedEvent( key, 1 ) );
						break;
					}
			}
		} );


		glfwSetMouseButtonCallback( window_, [ ] ( GLFWwindow* window, int32_t button, int32_t action, int32_t mods )
		{
			WindowData& windowData = *( static_cast<WindowData*>( glfwGetWindowUserPointer( window ) ) );

			switch (action)
			{
				case GLFW_PRESS:
					{
						windowData.Listener( MouseButtonPressedEvent( button ) );
						break;
					}
				case GLFW_RELEASE:
					{
						windowData.Listener( MouseButtonReleasedEvent( button ) );
						break;
					}
			}
		} );

		glfwSetCursorPosCallback( window_, [ ] ( GLFWwindow* window, double_t x, double_t y )
		{
			WindowData& windowData = *( static_cast<WindowData*>( glfwGetWindowUserPointer( window ) ) );
			windowData.Listener( MouseMovedEvent( static_cast<float_t>( x ), static_cast<float_t>( y ) ) );
		} );

		glfwSetScrollCallback( window_, [ ] ( GLFWwindow* window, double_t xOffset, double_t yOffset )
		{
			WindowData& windowData = *( static_cast<WindowData*>( glfwGetWindowUserPointer( window ) ) );
			windowData.Listener( MouseScrolledEvent( static_cast<float_t>( xOffset ), static_cast<float_t>( yOffset ) ) );
		} );
	}

	void Window::PollEvents( )
	{
		glfwPollEvents( );
	}

	void Window::SwapBuffers( )
	{
		glfwSwapBuffers( window_ );
	}

	void Window::SetVsync( bool value )
	{
		glfwSwapInterval( value );
		isVsync_ = value;
	}

	void Window::Destroy( )
	{
		glfwDestroyWindow( window_ );
		glfwTerminate( );
	}
}

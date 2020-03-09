#include "ilpch.h"
#include "Window.h"

#include "Log.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "illithid/events/WindowEvents.h"

namespace itd
{
	Window::Window( const WindowProperties& props )
		: isVsync_( true ), window_( nullptr )
	{
		windowData_.Width = props.Width;
		windowData_.Height = props.Height;
		windowData_.Title = props.Title;

		this->Initialize( );
	}

	Window::~Window( )
	{
		this->Cleanup( );
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

		glfwSetWindowUserPointer( window_, static_cast<void*>( &windowData_ ) );

		glfwSetWindowCloseCallback( window_, [ ] ( GLFWwindow* window )
		{
			WindowData& windowData = *( static_cast<WindowData*>( glfwGetWindowUserPointer( window ) ) );
			windowData.Listener( WindowClosedEvent( ) );
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

	void Window::Cleanup( )
	{
		glfwDestroyWindow( window_ );
		glfwTerminate( );
	}
}

#include "ilpch.h"
#include "Application.h"

#include "Log.h"
#include "Window.h"
#include "Macros.h"
#include "illithid/renderer/Graphics.h"
#include "glad/glad.h"

namespace itd
{

	Application::Application( ) :
		isRunning_( true )
	{
		this->Initialize( );
	}

	Application::~Application( )
	{
		this->Destroy( );
	}

	void Application::Run( )
	{
		this->Start( );

		while (isRunning_)
		{
			Graphics::ClearColor( 1.0f, 1.0f, 0.0f, 1.0f );
			Graphics::Clear( GL_COLOR_BUFFER_BIT );

			window_->SwapBuffers( );
			window_->PollEvents( );
		}

		this->Shutdown( );
	}

	void Application::OnEvent( Event& event )
	{
		EventDispatcher dispatcher( event );
		dispatcher.Dispatch<WindowClosedEvent>( BIND_EVENT_FUNCTION( Application::OnWindowClosed, this ) );
	}

	void Application::Initialize( )
	{
		//Change this to take a log-level
		Log::Initialize( );

		window_ = std::make_unique<Window>( WindowProperties( ) );
		window_->SetEventListener( BIND_EVENT_FUNCTION( Application::OnEvent, this ) );

		Graphics::Initialize( window_ );
	}

	void Application::Destroy( )
	{
		Graphics::Destroy( );
	}

	bool Application::OnWindowClosed( WindowClosedEvent& event )
	{
		IL_CORE_INFO( "Closing Window.." );

		isRunning_ = false;
		return true;
	}
}
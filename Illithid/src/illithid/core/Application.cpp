#include "ilpch.h"
#include "Application.h"

#include "Log.h"
#include "Time.h"
#include "Window.h"
#include "Macros.h"
#include "FileSystem.h"
#include "illithid/renderer/Graphics.h"

namespace itd
{
	double_t Time::elapsed_ = 0.0;

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
		using TimePoint = std::chrono::steady_clock::time_point;

		TimePoint startTime = std::chrono::high_resolution_clock::now( );
		TimePoint currentTime;

		this->Start( );
		Graphics::EnableCapabality( Capability::Cap_DepthTest );

		while (isRunning_)
		{
			currentTime = std::chrono::high_resolution_clock::now( );
			Time::elapsed_ = ( std::chrono::duration_cast<std::chrono::microseconds>( currentTime - startTime ).count( ) ) / 1000000.0;


			Graphics::ClearColor( 0.0f, 0.5f, 0.5f, 1.0f );
			Graphics::Clear( BufferBit::BB_Color | BufferBit::BB_Depth );

			this->Render( );

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

		FileSystem::Initialize( );

		window_ = std::make_unique<Window>( WindowProperties( "GL", 800, 800 ) );
		window_->SetEventListener( BIND_EVENT_FUNCTION( Application::OnEvent, this ) );

		Graphics::Initialize( window_ );
	}

	void Application::Destroy( )
	{
	}

	bool Application::OnWindowClosed( WindowClosedEvent& event )
	{
		IL_CORE_INFO( "Closing Window.." );

		isRunning_ = false;
		return true;
	}
}
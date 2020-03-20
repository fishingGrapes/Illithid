#include "ilpch.h"
#include "Application.h"

#include "Log.h"
#include "Time.h"
#include "Screen.h"
#include "Window.h"
#include "Macros.h"
#include "FileSystem.h"
#include "Input.h"
#include "illithid/renderer/Graphics.h"

namespace itd
{
	float_t Time::elapsed_ = 0.0f;
	float_t Time::delta_ = 0.0f;

	int32_t Screen::width_ = 0;
	int32_t Screen::height_ = 0;

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
		TimePoint previousTime = startTime;
		TimePoint currentTime;

		this->Start( );
		Graphics::EnableCapabality( Capability::Cap_DepthTest );

		while (isRunning_)
		{
			currentTime = std::chrono::high_resolution_clock::now( );
			Time::delta_ = ( std::chrono::duration_cast<std::chrono::microseconds>( currentTime - previousTime ).count( ) ) / 1000000.0f;
			Time::elapsed_ = ( std::chrono::duration_cast<std::chrono::microseconds>( currentTime - startTime ).count( ) ) / 1000000.0f;

			Graphics::ClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
			Graphics::Clear( BufferBit::BB_Color | BufferBit::BB_Depth );

			this->Render( );

			window_->SwapBuffers( );
			window_->PollEvents( );

			previousTime = currentTime;
		}

		this->Shutdown( );
	}

	void Application::OnEvent( Event& event )
	{
		EventDispatcher dispatcher( event );
		dispatcher.Dispatch<WindowClosedEvent>( BIND_EVENT_FUNCTION( Application::OnWindowClosed, this ) );
		dispatcher.Dispatch<WindowResizedEvent>( BIND_EVENT_FUNCTION( Application::OnWindowResized, this ) );

		Input::ProcessEvent( event );
	}

	void Application::Initialize( )
	{
		//Change this to take a log-level
		Log::Initialize( );

		FileSystem::Initialize( );

		window_ = std::make_unique<Window>( WindowProperties( "GL", 1920, 1080 ) );
		window_->SetEventListener( BIND_EVENT_FUNCTION( Application::OnEvent, this ) );

		Screen::width_ = window_->Width( );
		Screen::height_ = window_->Height( );

		Graphics::Initialize( window_ );
		Input::Initialize( );
	}

	void Application::Destroy( )
	{
		Input::Destroy( );
	}

	bool Application::OnWindowClosed( WindowClosedEvent& event )
	{
		IL_CORE_INFO( "Closing Window.." );

		isRunning_ = false;
		return true;
	}
	bool Application::OnWindowResized( WindowResizedEvent& event )
	{
		Screen::width_ = event.Width( );
		Screen::height_ = event.Height( );

		return false;
	}
}
#include "ilpch.h"
#include "Application.h"

#include "Log.h"
#include "Window.h"
#include "Macros.h"

namespace itd
{
	Application::Application( ) :
		isRunning_( true )
	{
		this->Initialize( );
	}

	Application::~Application( )
	{

	}

	void Application::Run( )
	{
		while (isRunning_)
		{
			window_->SwapBuffers( );
			window_->PollEvents( );
		}
	}

	void Application::OnEvent( Event& event )
	{
		EventDispatcher dispatcher( event );
		dispatcher.Dispatch<WindowClosedEvent>( std::bind( &Application::OnWindowClosed, this, std::placeholders::_1 ) );
	}

	void Application::Initialize( )
	{
		//Change this to take a log-level
		Log::Initialize( );
		IL_CORE_INFO( "Logger Initialized." );

		window_ = std::make_unique<Window>( WindowProperties( ) );
		window_->SetEventListener( std::bind( &Application::OnEvent, this, std::placeholders::_1 ) );
		IL_CORE_INFO( "Window Initialized." );

	}
	bool Application::OnWindowClosed( WindowClosedEvent& event )
	{
		IL_CORE_INFO( "Window Closed." );

		isRunning_ = false;
		return true;
	}
}
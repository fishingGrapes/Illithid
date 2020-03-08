#include "ilpch.h"
#include "Application.h"

#include "Log.h"
#include "platform/opengl/GLWindow.h"

namespace itd
{
	Application::Application( )
	{
		this->Initialize( );
	}

	Application::~Application( )
	{
	
	}

	void Application::Run( )
	{
		while (true)
		{
			window_->Update( );
		}
	}
	void Application::Initialize( )
	{
		//Change this to take a log-level
		Log::Initialize( );
		IL_CORE_INFO( "Logger Initialized." );

		window_ =std::make_unique<GLWindow>( WindowProperties( ) );
		IL_CORE_INFO( "Window Initialized." );

	}
}
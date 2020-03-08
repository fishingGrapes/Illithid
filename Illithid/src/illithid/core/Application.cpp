#include "ilpch.h"
#include "Application.h"

#include "Log.h"

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
		while (true);
	}
	void Application::Initialize( )
	{
		//Change this to take a log-level
		Log::Initialize( );

		IL_CORE_INFO( "Logger Initialized." );
	}
}
#pragma once

#include <memory>
#include "illithid/events/WindowEvents.h"

namespace itd
{
	class Window;

	class Application
	{
	public:
		Application( );
		virtual ~Application( );

		void Run( );

	protected:
		virtual void Start( ) = 0;
		virtual void Shutdown( ) = 0;
		virtual void Render( ) = 0;

		virtual void OnEvent( Event& event );

	private:
		void Initialize( );
		void Destroy( );
		bool OnWindowClosed( WindowClosedEvent& event );

		std::unique_ptr<Window> window_;
		bool isRunning_;
	};

	//Just the Declaration, defined in client with the child class
	Application* CreateApplication( );
}

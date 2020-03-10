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
		void OnEvent( std::shared_ptr<Event> event );


		virtual void Start( ) = 0;
		virtual void Shutdown( ) = 0;
		virtual void Tick( double_t at, double_t dt ) = 0;
		virtual void FixedTick( double_t dt ) = 0;
		virtual void LateTick( double_t at, double_t dt ) = 0;

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

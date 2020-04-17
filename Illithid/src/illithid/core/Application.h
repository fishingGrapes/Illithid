#pragma once

#include <memory>
#include "illithid/events/WindowEvents.h"

class FrameRateCounter;

namespace itd
{
	class Window;

	class Application
	{
	public:
		Application( );
		virtual ~Application( );

		void Run( );

		inline int32_t FrameRate( ) const;

	protected:
		virtual void Start( ) = 0;
		virtual void Shutdown( ) = 0;

		virtual void Update( ) = 0;
		virtual void PreRender( ) = 0;

		virtual void OnEvent( Event& event );

	private:
		void Initialize( );
		void Destroy( );
		void RegisterEngineComponents( );
		bool OnWindowClosed( WindowClosedEvent& event );
		bool OnWindowResized( WindowResizedEvent& event );

		std::unique_ptr<Window> window_;
		std::unique_ptr<FrameRateCounter> fpsCounter_;
		bool isRunning_;

	};

	//Just the Declaration, defined in client with the child class
	Application* CreateApplication( );
}

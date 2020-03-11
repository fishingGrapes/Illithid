#include "Illithid.h"
#include "illithid/core/EntryPoint.h"

using Event = itd::Event;
using EventDispatcher = itd::EventDispatcher;
using KeyPressedEvent = itd::KeyPressedEvent;
using KeyReleasedEvent = itd::KeyReleasedEvent;
using MouseButtonPressedEvent = itd::MouseButtonPressedEvent;
using MouseButtonReleasedEvent = itd::MouseButtonReleasedEvent;

class TestApplication : public itd::Application
{
public:
	TestApplication( )
	{
	}

	~TestApplication( )
	{
	}

	// Inherited via Application
	virtual void Start( ) override
	{
		IL_TRACE( "Start" );
	}

	virtual void Shutdown( ) override
	{
		IL_TRACE( "Shutdown" );
	}

	virtual void OnEvent( Event& event ) override
	{
		Application::OnEvent( event );

		EventDispatcher dispatcehr( event );
		dispatcehr.Dispatch<KeyPressedEvent>( BIND_EVENT_FUNCTION( TestApplication::OnKeyPressed, this ) );
	}

	bool OnKeyPressed( KeyPressedEvent& event )
	{
		IL_TRACE( event );
		return true;
	}
};

SET_ILLITHID_APPLICATION( TestApplication );

#include "Illithid.h"
#include "illithid/core/EntryPoint.h"

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
		//IL_TRACE( "Start" );
	}

	virtual void Shutdown( ) override
	{
		//IL_TRACE( "Shutdown" );
	}

	virtual void Tick( double_t at, double_t dt ) override
	{
		//IL_TRACE( "Tick" );

	}

	virtual void FixedTick( double_t dt ) override
	{
		//IL_TRACE( "FixedTick" );

		if (itd::Input::IsKeyDown( itd::KeyCode::SPACE ))
		{
			IL_TRACE( "Space is down" );
		}

		if (itd::Input::IsKeyUp( itd::KeyCode::SPACE ))
		{
			IL_TRACE( "Space is up" );
		}

		if (itd::Input::IsKeyHeld( itd::KeyCode::SPACE ))
		{
			//IL_TRACE( "Space is Held" );
		}

		if (itd::Input::IsMouseButtonDown( 0 ))
		{
			IL_TRACE( "LMB is down" );
		}

		if (itd::Input::IsMouseButtonUp( 0 ))
		{
			IL_TRACE( "LMB is up" );
		}

		if (itd::Input::IsMouseButtonHeld( 0 ))
		{
			//IL_TRACE( "LMB is Held" );
		}

		IL_TRACE( "Mouse Position {0}", itd::Input::GetMouseScrollDelta( ) );
	}

	virtual void LateTick( double_t at, double_t dt ) override
	{
		//IL_TRACE( "LateTick" );
	}
};

SET_ILLITHID_APPLICATION( TestApplication );

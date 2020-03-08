#include "Illithid/core/Application.h"
#include "Illithid/core/EntryPoint.h"

class TestApplication : public itd::Application
{
public:
	TestApplication( )
	{
	}

	~TestApplication( )
	{
	}
};

itd::Application* itd::CreateApplication( )
{
	return new TestApplication( );
}
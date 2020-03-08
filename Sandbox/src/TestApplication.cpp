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

SET_ILLITHID_APPLICATION( TestApplication );

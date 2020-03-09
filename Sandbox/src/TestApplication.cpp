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
};

SET_ILLITHID_APPLICATION( TestApplication );

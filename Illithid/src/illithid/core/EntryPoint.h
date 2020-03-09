#pragma once

#ifdef IL_PLATFORM_WINDOWS

//Trust me, this function is defined somewhere
extern itd::Application* itd::CreateApplication( );

int main( int argc, char** argv )
{
	auto app = itd::CreateApplication( );
	app->Run( );
	delete app;
}

#else

#error Illithid does not support this platform.

#endif
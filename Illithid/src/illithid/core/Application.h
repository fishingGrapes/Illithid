#pragma once
#include <memory>

namespace itd
{
	class Window;
	class Application
	{
	public:
		Application( );
		virtual ~Application( );

		void Run( );

	private:
		void Initialize( );

		std::unique_ptr<Window> window_;
	};

	//Just the Declaration, defined in client with the child class
	Application* CreateApplication( );
}

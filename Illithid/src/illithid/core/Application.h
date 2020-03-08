#pragma once

namespace itd
{
	class Application
	{
	public:
		Application( );
		virtual ~Application( );

		void Run( );

	private:
		void Initialize();
	};

	//Just the Declaration, defined in client with the child class
	Application* CreateApplication( );
}

#pragma once
#include "illithid/core/Window.h"

struct GLFWwindow;

namespace itd
{
	class GLWindow : public Window
	{
	public:
		explicit GLWindow( const WindowProperties& props );
		~GLWindow( );

		virtual void Update( ) override;

	private:
		void Initialize( );
		void Destroy();

		GLFWwindow* window_;
	};
}
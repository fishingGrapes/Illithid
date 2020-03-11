#include "ilpch.h"
#include "Graphics.h"

#include "illithid/core/Window.h"
#include "illithid/core/Log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace itd
{
	void Graphics::Initialize( std::unique_ptr<Window>& window )
	{
		GLFWwindow* glwindow = static_cast<GLFWwindow*>( window->GetRawWindow( ) );
		glfwMakeContextCurrent( glwindow );

		if (!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ))
		{
			IL_CORE_CRITICAL( "Failed to load OpenGL functions." );
			return;
		}

		glViewport( 0, 0, window->Width( ), window->Height( ) );
		glfwSetFramebufferSizeCallback( glwindow, [ ] ( GLFWwindow* glwindow, int32_t width, int32_t height )
		{
			IL_CORE_WARN( "Framebuffer resized to ({0}, {1})", width, height );
			glViewport( 0, 0, width, height );
		} );
	}

	void Graphics::Destroy( )
	{
	}

	void Graphics::Clear( uint32_t bits )
	{
		glClear( bits );
	}

	void Graphics::ClearColor( float_t r, float_t g, float_t b, float_t a )
	{
		glClearColor( r, g, b, a );
	}
}
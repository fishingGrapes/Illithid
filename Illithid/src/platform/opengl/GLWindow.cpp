#include "ilpch.h"
#include "GLWindow.h"

#include "illithid/core/Log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace itd
{
	GLWindow::GLWindow( const WindowProperties& props ) :
		Window( props )
	{
		this->Initialize( );

	}

	GLWindow::~GLWindow( )
	{
		this->Destroy( );
	}

	void GLWindow::Initialize( )
	{
		glfwInit( );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		window_ = glfwCreateWindow( windowProps_.Width, windowProps_.Height, windowProps_.Title.c_str( ), nullptr, nullptr );
		if (window_ == nullptr)
		{
			IL_CORE_CRITICAL( "Failed to create GLFW window." );
			glfwTerminate( );
			return;
		}
		glfwMakeContextCurrent( window_ );

		if (!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ))
		{
			IL_CORE_CRITICAL( "Failed to load opengl function addresses." );
			glfwTerminate( );
			return;
		}

		glViewport( 0, 0, windowProps_.Width, windowProps_.Height );
	}

	void GLWindow::Destroy( )
	{
		glfwTerminate( );
	}

	void GLWindow::Update( )
	{
		glClearColor( 1.0f, 0.0f, 0.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		glfwSwapBuffers( window_ );
		glfwPollEvents( );
	}


}
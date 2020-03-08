#include "ilpch.h"
#include "Application.h"

#include "Log.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace itd
{
	Application::Application( )
	{
		this->Initialize( );
	}
	Application::~Application( )
	{
	}

	void Application::Run( )
	{
		glfwInit( );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		GLFWwindow* window = glfwCreateWindow( 1280, 720, "Test Application", nullptr, nullptr );
		if (window == nullptr)
		{
			IL_CORE_CRITICAL( "Failed to create GLFW window." );
			glfwTerminate( );
			return;
		}

		glfwMakeContextCurrent( window );


		if (!gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ))
		{
			IL_CORE_ERROR( "Failed to create GLFW window." );
			glfwTerminate( );
			return;
		}

		glViewport( 0, 0, 1280, 720 );
		while (!glfwWindowShouldClose( window ))
		{
			glClearColor( 1.0f, 0.0f, 0.0f, 1.0f );
			glClear( GL_COLOR_BUFFER_BIT );

			glfwSwapBuffers( window );
			glfwPollEvents( );
		}

		glfwTerminate( );
	}
	void Application::Initialize( )
	{
		//Change this to take a log-level
		Log::Initialize( );
		IL_CORE_INFO( "Logger Initialized." );
	}
}
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
			glViewport( 0, 0, width, height );
		} );

#if defined(IL_DEBUG) || defined(IL_RELEASE)
		glEnable( GL_DEBUG_OUTPUT );
		glDebugMessageCallback( GLErrorCallback, nullptr );
#endif
	}

	void Graphics::Clear( int32_t bits )
	{
		glClear( bits );
	}

	void Graphics::ClearColor( float_t r, float_t g, float_t b, float_t a )
	{
		glClearColor( r, g, b, a );
	}

	void Graphics::EnableCapabality( int32_t capability )
	{
		glEnable( capability );
	}

	void Graphics::DisableCapabality( int32_t capability )
	{
		glDisable( capability );
	}

	void Graphics::SetPolygonMode( PolygonFace face, PolygonMode mode )
	{
		glPolygonMode( face, mode );
	}

	void Graphics::DrawMesh( StaticMesh& mesh, Material& material )
	{
		material.Use( );
		mesh.Bind( );

		//glDrawElements( GL_TRIANGLES, static_cast<GLsizei>( mesh.ElementCount( ) ), GL_UNSIGNED_INT, nullptr );
		glDrawArrays( GL_TRIANGLES, 0, static_cast<GLsizei>( mesh.VertexCount( ) ) );
	}


	void Graphics::GLErrorCallback( GLenum source, GLenum type, GLenum id, GLenum severity, GLsizei length,
									const GLchar* message, const void* userParam )
	{
		std::string msgsrc;
		switch (source)
		{
			case GL_DEBUG_SOURCE_API:
				msgsrc = "OpenGL API";
				break;

			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				msgsrc = "Window System API";
				break;

			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				msgsrc = "Shader Compiler";
				break;

			case GL_DEBUG_SOURCE_THIRD_PARTY:
				msgsrc = "Third-Party API";
				break;

			case GL_DEBUG_SOURCE_APPLICATION:
				msgsrc = "Application API";
				break;

			case GL_DEBUG_SOURCE_OTHER:
				msgsrc = "Other Source";
				break;

		}

		std::string msgtype;
		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR:
				msgtype = "Error Message";
				break;

			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				msgtype = "Deprecated Behaviour";
				break;

			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				msgtype = "Undefined Behaviour";
				break;

			case GL_DEBUG_TYPE_PORTABILITY:
				msgtype = "Portability Warning";
				break;

			case GL_DEBUG_TYPE_PERFORMANCE:
				msgtype = "Performance Warning";
				break;

			case GL_DEBUG_TYPE_MARKER:
				msgtype = "Command stream annotation";
				break;

			case GL_DEBUG_TYPE_OTHER:
				msgtype = "Other message";
				break;

			default:
				msgtype = "Grouping message";
				break;
		}

		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				IL_CORE_ERROR( "{0} :: {1} :: {2}", msgsrc, msgtype, message );
				break;

			case GL_DEBUG_SEVERITY_MEDIUM:
				IL_CORE_WARN( "{0} :: {1} :: {2}", msgsrc, msgtype, message );
				break;

			case GL_DEBUG_SEVERITY_LOW:
				IL_CORE_INFO( "{0} :: {1} :: {2}", msgsrc, msgtype, message );
				break;

			case GL_DEBUG_SEVERITY_NOTIFICATION:
				IL_CORE_TRACE( "{0} :: {1} :: {2}", msgsrc, msgtype, message );
				break;
		}
	}
}
#include "ilpch.h"
#include "Graphics.h"

#include "illithid/core/Window.h"
#include "illithid/core/Log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "illithid/components/Light.h"
#include "illithid/components/Camera.h"
#include "illithid/renderer/Skybox.h"

namespace itd
{

	std::multiset<MeshRenderer*, Graphics::Comparator> Graphics::renderGraph_;
	std::unique_ptr<Skybox> Graphics::skybox_;

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

	void Graphics::DrawLineSegment( LineSegment& mesh, Material& material )
	{
		material.Use( );
		mesh.Bind( );

		glDrawArrays( GL_LINES, 0, static_cast<GLsizei>( 2 ) );
	}

	void Graphics::BuildRenderGraph( )
	{
		renderGraph_.clear( );

		auto& renderers = *( MeshRenderer::get_allocator( ) );
		for (size_t i = 0; i < renderers.size( ); ++i)
		{
			renderGraph_.insert( renderers[ i ] );
		}
	}

	void Graphics::DrawRenderGraph( )
	{

		std::shared_ptr<Material> first_material = ( *( renderGraph_.begin( ) ) )->Material;
		uint32_t prev_program = first_material->ProgramID( );
		first_material->Use( );

		auto& lights = *( Light::get_allocator( ) );

		for (auto it = renderGraph_.begin( ); it != renderGraph_.end( ); ++it)
		{
			MeshRenderer* renderer = *( it );
			std::shared_ptr<Material> material = renderer->Material;
			std::shared_ptr<StaticMesh> mesh = renderer->Mesh;
			uint32_t current_program = material->ProgramID( );

			//Switch shaders only when necessary
			if (current_program != prev_program)
			{
				material->Use( );
				prev_program = current_program;

				if (material->HasLightingPass)
				{
					material->SetVector3f( "u_ViewPosition", Camera::Primary( )->gameObject->GetTransform( )->Position );
				}
			}

			material->SetMatrix4f( "u_ViewProjection", Camera::Primary( )->ViewProjection( ) );
			material->SetMatrix4f( "u_Model", renderer->gameObject->GetTransform( )->TRS( ) );

			mesh->Bind( );
			if (material->HasLightingPass)
			{
				material->SetMatrix3f( "u_NormalMatrix", glm::mat3( glm::transpose( renderer->gameObject->GetTransform( )->InverseTRS( ) ) ) );

				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
				glDepthFunc( GL_LEQUAL );

				for (size_t i = 0; i < lights.size( ); ++i)
				{

					Light* light = lights[ i ];
					GLuint lightingFunctionIndex = 0;

					switch (light->Type)
					{
						case LightType::Directional:
							{
								material->SetVector3f( "u_DirectionalLight.direction", glm::vec3( -0.2f, -1.0f, -0.3f ) );
								material->SetVector3f( "u_DirectionalLight.ambient", 0.005f * light->Color );
								material->SetVector3f( "u_DirectionalLight.diffuse", 0.4f * light->Color );
								material->SetVector3f( "u_DirectionalLight.specular", 0.5f * light->Color );

								lightingFunctionIndex = glGetSubroutineIndex( static_cast<GLuint>( material->ProgramID( ) ), GL_FRAGMENT_SHADER, "CalculateDirectionLight" );
								break;
							}
						case LightType::Point:
							{
								material->SetVector3f( "u_PointLight.position", light->gameObject->GetTransform( )->Position );

								material->SetVector3f( "u_PointLight.ambient", 0.005f * light->Color );
								material->SetVector3f( "u_PointLight.diffuse", 0.8f * light->Color );
								material->SetVector3f( "u_PointLight.specular", 1.0f * light->Color );

								material->SetFloat( "u_PointLight.constant", 1.0f );
								material->SetFloat( "u_PointLight.linear", 0.009f );
								material->SetFloat( "u_PointLight.quadratic", 0.032f );

								lightingFunctionIndex = glGetSubroutineIndex( material->ProgramID( ), GL_FRAGMENT_SHADER, "CalculatePointLight" );
								break;
							}
						case LightType::Spot:
							{
								material->SetVector3f( "u_SpotLight.ambient", glm::vec3( 0.0f ) );
								material->SetVector3f( "u_SpotLight.diffuse", glm::vec3( 1.0f ) );
								material->SetVector3f( "u_SpotLight.specular", glm::vec3( 1.0f ) );

								material->SetFloat( "u_SpotLight.constant", 1.0f );
								material->SetFloat( "u_SpotLight.linear", 0.009f );
								material->SetFloat( "u_SpotLight.quadratic", 0.032f );

								material->SetFloat( "u_SpotLight.innerCutoff", glm::cos( glm::radians( 12.5f ) ) );
								material->SetFloat( "u_SpotLight.outerCutoff", glm::cos( glm::radians( 15.0f ) ) );

								material->SetVector3f( "u_SpotLight.position", light->gameObject->GetTransform( )->Position );
								material->SetVector3f( "u_SpotLight.direction", light->gameObject->GetTransform( )->Forward( ) );

								lightingFunctionIndex = glGetSubroutineIndex( material->ProgramID( ), GL_FRAGMENT_SHADER, "CalculateSpotLight" );
								break;
							}
					}


					glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &lightingFunctionIndex );
					glDrawArrays( GL_TRIANGLES, 0, static_cast<GLsizei>( mesh->VertexCount( ) ) );
					glBlendFunc( GL_ONE, GL_ONE );
				}

				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
				glDepthFunc( GL_LESS );
			}
			else
			{
				glDrawArrays( GL_TRIANGLES, 0, static_cast<GLsizei>( mesh->VertexCount( ) ) );
			}
		}

	}

	void Graphics::SetSkybox( const std::string& file )
	{
		skybox_.reset( new Skybox( file ) );
	}

	void Graphics::RenderSkyBox( )
	{
		skybox_->Render( );
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
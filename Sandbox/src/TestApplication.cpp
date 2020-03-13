#include "Illithid.h"
#include "illithid/core/EntryPoint.h"

using Event = itd::Event;
using EventDispatcher = itd::EventDispatcher;
using KeyPressedEvent = itd::KeyPressedEvent;
using KeyReleasedEvent = itd::KeyReleasedEvent;
using MouseButtonPressedEvent = itd::MouseButtonPressedEvent;
using MouseButtonReleasedEvent = itd::MouseButtonReleasedEvent;
using MouseMovedEvent = itd::MouseMovedEvent;

using Graphics = itd::Graphics;
using Shader = itd::Shader;
using StaticMesh = itd::StaticMesh;
using Vertex = itd::Vertex;
using Material = itd::Material;

class TestApplication : public itd::Application
{

private:
	std::unique_ptr<Material> material_;
	std::unique_ptr < StaticMesh> mesh_;

	glm::vec2 mousePosition_;

public:

	TestApplication( )
		:mousePosition_( 0.0f )
	{
	}

	~TestApplication( )
	{
	}

	// Inherited via Application
	virtual void Start( ) override
	{
		IL_TRACE( "Start" );

		Shader shader( "Assets/Shaders/basic.shader" );
		material_ = std::make_unique<Material>( shader );
		material_->SetVector2f( "u_Resolution", glm::vec2( 800, 800 ) );


		std::vector<Vertex> vertices;
		vertices.emplace_back( Vertex{ glm::vec3( -1.0f, 1.0f, 0.0f ), glm::vec4( 0.0f, 1.0f, 1.0f, 1.0f ) } );
		vertices.emplace_back( Vertex{ glm::vec3( -1.0f, -1.0f, 0.0f ),  glm::vec4( 0.0f, 1.0f, 1.0f, 1.0f ) } );
		vertices.emplace_back( Vertex{ glm::vec3( 1.0f, -1.0f, 0.0f ),  glm::vec4( 0.0f, 1.0f, 1.0f, 1.0f ) } );
		vertices.emplace_back( Vertex{ glm::vec3( 1.0f, -1.0f, 0.0f ),  glm::vec4( 0.0f, 1.0f, 1.0f, 1.0f ) } );
		vertices.emplace_back( Vertex{ glm::vec3( 1.0f, 1.0f, 0.0f ), glm::vec4( 0.0f, 1.0f, 1.0f, 1.0f ) } );
		vertices.emplace_back( Vertex{ glm::vec3( -1.0f, 1.0f, 0.0f ),  glm::vec4( 0.0f, 1.0f, 1.0f, 1.0f ) } );

		mesh_ = std::make_unique<StaticMesh>( std::move( vertices ) );
	}

	virtual void Shutdown( ) override
	{
		IL_TRACE( "Shutdown" );
	}

	virtual void OnEvent( Event& event ) override
	{
		Application::OnEvent( event );

		EventDispatcher dispatcher( event );
		dispatcher.Dispatch<MouseMovedEvent>( [ this ] ( MouseMovedEvent& evnt ) -> bool
		{
			mousePosition_.x = evnt.X( );
			mousePosition_.y = evnt.Y( );

			return false;
		} );
	}



	// Inherited via Application
	virtual void Render( ) override
	{
		//material_->SetFloat( "u_Time", static_cast<float_t>( itd::Time::Elapsed( ) ) );
		//material_->SetVector2f( "u_Mouse", mousePosition_ );

		Graphics::DrawMesh( *mesh_, *material_ );
	}
};

SET_ILLITHID_APPLICATION( TestApplication );

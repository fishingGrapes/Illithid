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
using Texture2D = itd::Texture2D;

using Transform = itd::Transform;

class TestApplication : public itd::Application
{

private:
	std::unique_ptr<Material> material_;
	std::shared_ptr<StaticMesh> mesh_;
	std::shared_ptr<Texture2D> containerTexture_;

	glm::vec2 mousePosition_;
	Transform transform_;
	double_t rotation_;

public:

	TestApplication( )
		:mousePosition_( 0.0f ), rotation_( 0.0 )
	{
	}

	~TestApplication( )
	{
	}

	// Inherited via Application
	virtual void Start( ) override
	{
		containerTexture_ = Texture2D::Load( "Assets/Textures/box.tex2D" );

		Shader shader( "Assets/Shaders/basic.shader" );
		material_ = std::make_unique<Material>( shader );
		material_->SetTexture( "u_BrickWall", containerTexture_ );


		mesh_ = StaticMesh::Load( "Assets/Models/box.obj" );
		Graphics::SetPolygonMode( itd::PolygonFace::PF_FrontAndBack, itd::PolygonMode::PM_Fill );
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

		dispatcher.Dispatch<KeyPressedEvent>( [ this ] ( KeyPressedEvent& evnt )-> bool
		{
			switch (evnt.KeyCode( ))
			{
				case itd::KeyCode::W:
					{
						transform_.Translate( glm::vec3( 0.0f, 0.0f, -0.01f ) );
						break;
					}

				case itd::KeyCode::S:
					{
						transform_.Translate( glm::vec3( 0.0f, 0.0f, 0.01f ) );
						break;
					}

				case itd::KeyCode::A:
					{
						transform_.Translate( glm::vec3( 0.05f, 0.0f, 0.0f ) );
						break;
					}

				case itd::KeyCode::D:
					{
						transform_.Translate( glm::vec3( -0.05f, 0.0f, 0.0f ) );
						break;
					}
			}

			return false;
		} );
	}


	// Inherited via Application
	virtual void Render( ) override
	{
		transform_.Rotate( glm::vec3( 0.0025f, 0.005f, 0.0f ) );

		transform_.Update( );
		material_->SetMatrix4f( "u_Model", transform_.TRS( ) );
		Graphics::DrawMesh( *mesh_, *material_ );
	}
};

SET_ILLITHID_APPLICATION( TestApplication );

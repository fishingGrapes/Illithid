#include "Illithid.h"
#include "illithid/core/EntryPoint.h"

using Screen = itd::Screen;
using Time = itd::Time;
using Input = itd::Input;

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
using Camera = itd::Camera;
using PerspectiveProjection = itd::PerspectiveProjection;
using OrthographicProjection = itd::OrthographicProjection;

class TestApplication : public itd::Application
{

private:
	std::unique_ptr<Material> material_;
	std::shared_ptr<StaticMesh> mesh_;
	std::shared_ptr<Texture2D> boxTexture_;

	glm::vec2 mousePosition_;
	glm::vec3 rotation_;

	Transform camTransform_, meshTransform_;
	Camera camera_;

public:

	TestApplication( )
		:mousePosition_( 0.0f ), rotation_( glm::vec3( 0.0 ) )
	{
	}

	~TestApplication( )
	{
	}

	// Inherited via Application
	virtual void Start( ) override
	{
		boxTexture_ = Texture2D::Load( "Assets/Textures/box.tex2D" );

		Shader shader( "Assets/Shaders/basic.shader" );
		material_ = std::make_unique<Material>( shader );
		material_->SetTexture( "u_BrickWall", boxTexture_ );

		mesh_ = StaticMesh::Load( "Assets/Models/box.obj" );
		Graphics::SetPolygonMode( itd::PolygonFace::PF_Front, itd::PolygonMode::PM_Fill );

		camera_.SetPerspectiveProjection( PerspectiveProjection{ glm::radians( 60.0f ), Screen::Width( ) / static_cast<float_t>( Screen::Height( ) ), 0.1f, 100.0f } );
		camTransform_.Translate( glm::vec3( 0.0f, 0.0f, 1.0f ) );

		mousePosition_ = glm::vec2( Screen::Width( ) * 0.5f, Screen::Height( ) * 0.5f );
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
			float_t deltaX = mousePosition_.x - evnt.X( );
			float_t deltaY = mousePosition_.y - evnt.Y( );

			mousePosition_.x = evnt.X( );
			mousePosition_.y = evnt.Y( );

			deltaX *= Time::Delta( ) * 0.45f;
			deltaY *= Time::Delta( ) * 0.45f;

			rotation_.x += deltaX;
			rotation_.y += deltaY;

			if (glm::abs( glm::degrees( rotation_.y ) ) > 90.0f)
			{
				deltaY = 0.0f;
				rotation_.y -= deltaY;
			}
			IL_TRACE( glm::abs( glm::degrees( rotation_.y ) ) );
			camTransform_.Rotate( glm::vec3( deltaY, deltaX, 0.0f ), itd::TransformationSpace::Camera );

			return false;
		} );
	}

	void MoveCamera( )
	{
		if (Input::IsKeyDown( itd::KeyCode::W ) || Input::IsKeyHeld( itd::KeyCode::W ))
		{
			camTransform_.Translate( Time::Delta( ) * camTransform_.Forward( ) * glm::vec3( 1.0f, 1.0f, -1.0f ) );
		}

		if (Input::IsKeyDown( itd::KeyCode::S ) || Input::IsKeyHeld( itd::KeyCode::S ))
		{
			camTransform_.Translate( Time::Delta( ) * camTransform_.Forward( ) * glm::vec3( -1.0f, -1.0f, 1.0f ) );
		}

		if (Input::IsKeyDown( itd::KeyCode::A ) || Input::IsKeyHeld( itd::KeyCode::A ))
		{
			camTransform_.Translate( Time::Delta( ) * camTransform_.Right( ) * glm::vec3( -1.0f, -1.0f, 1.0f ) );
		}

		if (Input::IsKeyDown( itd::KeyCode::D ) || Input::IsKeyHeld( itd::KeyCode::D ))
		{
			camTransform_.Translate( Time::Delta( ) * camTransform_.Right( ) * glm::vec3( 1.0f, 1.0f, -1.0f ) );
		}
	}


	// Inherited via Application
	virtual void Render( ) override
	{
		MoveCamera( );

		meshTransform_.Rotate( glm::vec3( 0.0075f, 0.0075f, 0.0f ) * Time::Delta( ) * 45.0f, itd::TransformationSpace::Local );

		camTransform_.Update( );
		meshTransform_.Update( );

		material_->SetMatrix4f( "u_Model", meshTransform_.TRS( ) );
		material_->SetMatrix4f( "u_ViewProjection", camera_.CalculateViewProjection( camTransform_.InverseTRS( ) ) );
		Graphics::DrawMesh( *mesh_, *material_ );
	}
};

SET_ILLITHID_APPLICATION( TestApplication );

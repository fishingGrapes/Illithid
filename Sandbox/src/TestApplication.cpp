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
using Vertex = itd::Vertex;
using Material = itd::Material;
using Texture2D = itd::Texture2D;

using PerspectiveProjection = itd::PerspectiveProjection;
using OrthographicProjection = itd::OrthographicProjection;

using GameObject = itd::GameObject;
using MeshRenderer = itd::MeshRenderer;
using Light = itd::Light;
using Transform = itd::Transform;
using Camera = itd::Camera;
using StaticMesh = itd::StaticMesh;
using LineSegment = itd::LineSegment;

class TestApplication : public itd::Application
{

private:

	glm::vec2 mousePosition_;
	glm::vec3 rotation_;

	std::unique_ptr<GameObject> cup_, camera_, light_;

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
		Shader basicShader( "Assets/Shaders/basic.shader" );
		std::shared_ptr<Material> basicMat_ = std::make_unique<Material>( basicShader );

		Shader phongShader( "Assets/Shaders/phong.shader" );
		std::shared_ptr<Material> phongMat_ = std::make_unique<Material>( phongShader );

		cup_ = std::make_unique<GameObject>( );
		auto cupRenderer = cup_->AddComponent<MeshRenderer>( );
		cupRenderer->Mesh = StaticMesh::Load( "Assets/Models/cup.obj" );
		cupRenderer->Material = phongMat_;
		cupRenderer->Material->SetVector4f( "u_Color", glm::vec4( 1.0f, 0.5f, 0.31f, 1.0f ) );
		cupRenderer->Material->SetFloat( "u_AmbientStrength", 0.1f );
		cupRenderer->Material->SetFloat( "u_SpecularStrength", 0.5f );

		camera_ = std::make_unique<GameObject>( );
		camera_->GetTransform( )->Translate( glm::vec3( 0.0f, 0.0f, 1.0f ) );
		auto cam = camera_->AddComponent<Camera>( );
		cam->SetPerspectiveProjection( PerspectiveProjection{ glm::radians( 60.0f ), Screen::Width( ) / static_cast<float_t>( Screen::Height( ) ), 0.1f, 100.0f } );

		light_ = std::make_unique<GameObject>( );
		light_->GetTransform( )->Scale( glm::vec3( 0.25f ) );
		auto lightComp = light_->AddComponent<Light>( );
		lightComp->Color = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		auto lightRenderer = light_->AddComponent<MeshRenderer>( );
		lightRenderer->Mesh = StaticMesh::Load( "Assets/Models/box.obj" );
		lightRenderer->Material = basicMat_;
		lightRenderer->Material->SetVector4f( "u_Color", lightComp->Color );
		cupRenderer->Material->SetVector4f( "u_LightColor", lightComp->Color );

		mousePosition_ = glm::vec2( Screen::Width( ) * 0.5f, Screen::Height( ) * 0.5f );
		Camera::SetAsPrimary( cam );
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

			if (glm::abs( glm::degrees( rotation_.y ) ) > 70.0f)
			{
				deltaY = 0.0f;
				rotation_.y = glm::sign( glm::degrees( rotation_.y ) ) * glm::radians( 70.0f );
			}

			camera_->GetTransform( )->Rotate( glm::vec3( deltaY, deltaX, 0.0f ), itd::TransformationSpace::Camera );


			return false;
		} );

		dispatcher.Dispatch<itd::WindowResizedEvent>( [ this ] ( itd::WindowResizedEvent& evnt ) -> bool
		{
			camera_->GetComponent<Camera>( )->SetPerspectiveProjection( PerspectiveProjection{ glm::radians( 60.0f ), Screen::Width( ) / static_cast<float_t>( Screen::Height( ) ), 0.1f, 100.0f } );
			return false;
		} );
	}

	void MoveCamera( )
	{
		if (Input::IsKeyDown( itd::KeyCode::W ) || Input::IsKeyHeld( itd::KeyCode::W ))
		{
			camera_->GetTransform( )->Translate( Time::Delta( ) * camera_->GetTransform( )->Forward( ) );
		}

		if (Input::IsKeyDown( itd::KeyCode::S ) || Input::IsKeyHeld( itd::KeyCode::S ))
		{
			camera_->GetTransform( )->Translate( Time::Delta( ) * camera_->GetTransform( )->Forward( ) * -1.0f );
		}

		if (Input::IsKeyDown( itd::KeyCode::A ) || Input::IsKeyHeld( itd::KeyCode::A ))
		{
			camera_->GetTransform( )->Translate( Time::Delta( ) * camera_->GetTransform( )->Right( ) * -1.0f );
		}

		if (Input::IsKeyDown( itd::KeyCode::D ) || Input::IsKeyHeld( itd::KeyCode::D ))
		{
			camera_->GetTransform( )->Translate( Time::Delta( ) * camera_->GetTransform( )->Right( ) );
		}

	}

	// Inherited via Application
	virtual void Update( ) override
	{
		float_t angle = Time::Elapsed( ) * 0.5f;
		light_->GetTransform()->Position = glm::vec3( 2.0f * glm::cos( angle ), 0.0f, 2.0f * glm::sin( angle ) );

		MoveCamera( );

		auto cupRenderer = cup_->GetComponent<MeshRenderer>( );
		cupRenderer->Material->SetVector3f( "u_ViewPosition", camera_->GetTransform( )->Position );
		cupRenderer->Material->SetVector3f( "u_LightPosition", light_->GetTransform( )->Position );
	}


	// Inherited via Application
	virtual void PreRender( ) override
	{
		cup_->GetComponent<MeshRenderer>( )->Material->SetMatrix3f( "u_NormalMatrix", glm::mat3( glm::transpose( cup_->GetTransform( )->InverseTRS( ) ) ) );
	}

};

SET_ILLITHID_APPLICATION( TestApplication );

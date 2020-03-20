#include "Illithid.h"
#include "illithid/core/EntryPoint.h"

#include"SceneObjects.h"

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

class TestApplication : public itd::Application
{

private:
	std::unique_ptr<Material> basicMat_, phongMat_;
	std::shared_ptr<Texture2D> boxTexture_;

	glm::vec2 mousePosition_;
	glm::vec3 rotation_;

	MeshObject boxObject_;
	CameraObject cObject_;
	LightObject lObject_;

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

		Shader basicShader( "Assets/Shaders/basic.shader" );
		basicMat_ = std::make_unique<Material>( basicShader );

		Shader phongShader( "Assets/Shaders/phong.shader" );
		phongMat_ = std::make_unique<Material>( phongShader );

		//material_->SetTexture( "u_BrickWall", boxTexture_ );

		boxObject_.mesh = StaticMesh::Load( "Assets/Models/box.obj" );
		lObject_.mesh = StaticMesh::Load( "Assets/Models/box.obj" );

		cObject_.camera.SetPerspectiveProjection( PerspectiveProjection{ glm::radians( 60.0f ), Screen::Width( ) / static_cast<float_t>( Screen::Height( ) ), 0.1f, 100.0f } );
		cObject_.transform.Translate( glm::vec3( 0.0f, 0.0f, 1.0f ) );

		lObject_.transform.Translate( glm::vec3( 1.0f, 1.5f, -2.0f ) );
		lObject_.transform.Scale( glm::vec3( 0.25f ) );
		lObject_.color = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );

		mousePosition_ = glm::vec2( Screen::Width( ) * 0.5f, Screen::Height( ) * 0.5f );

		//Graphics::SetPolygonMode( itd::PolygonFace::PF_Front, itd::PolygonMode::PM_Fill );
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

			cObject_.transform.Rotate( glm::vec3( deltaY, deltaX, 0.0f ), itd::TransformationSpace::Camera );


			return false;
		} );
	}

	void MoveCamera( )
	{
		if (Input::IsKeyDown( itd::KeyCode::W ) || Input::IsKeyHeld( itd::KeyCode::W ))
		{
			cObject_.transform.Translate( Time::Delta( ) * cObject_.transform.Forward( ) );
		}

		if (Input::IsKeyDown( itd::KeyCode::S ) || Input::IsKeyHeld( itd::KeyCode::S ))
		{
			cObject_.transform.Translate( Time::Delta( ) * cObject_.transform.Forward( ) * -1.0f );
		}

		if (Input::IsKeyDown( itd::KeyCode::A ) || Input::IsKeyHeld( itd::KeyCode::A ))
		{
			cObject_.transform.Translate( Time::Delta( ) * cObject_.transform.Right( ) * -1.0f );
		}

		if (Input::IsKeyDown( itd::KeyCode::D ) || Input::IsKeyHeld( itd::KeyCode::D ))
		{
			cObject_.transform.Translate( Time::Delta( ) * cObject_.transform.Right( ) );
		}

		IL_TRACE( cObject_.transform.Forward( ) );
	}


	// Inherited via Application
	virtual void Render( ) override
	{
		MoveCamera( );
		boxObject_.transform.Rotate( glm::vec3( 0.0075f, 0.0075f, 0.0f ) * Time::Delta( ) * 45.0f, itd::TransformationSpace::Local );

		cObject_.transform.Update( );
		boxObject_.transform.Update( );
		lObject_.transform.Update( );

		basicMat_->SetMatrix4f( "u_ViewProjection", cObject_.camera.CalculateViewProjection( cObject_.transform.InverseTRS( ) ) );
		phongMat_->SetMatrix4f( "u_ViewProjection", cObject_.camera.CalculateViewProjection( cObject_.transform.InverseTRS( ) ) );

		phongMat_->SetVector4f( "u_Color", glm::vec4( 1.0f, 0.5f, 0.31f, 1.0f ) );
		phongMat_->SetVector4f( "u_LightColor", lObject_.color );
		phongMat_->SetFloat( "u_AmbientStrength", 0.1f );
		phongMat_->SetMatrix4f( "u_Model", boxObject_.transform.TRS( ) );
		phongMat_->SetMatrix3f( "u_NormalMatrix", glm::mat3( glm::transpose( boxObject_.transform.InverseTRS( ) ) ) );
		phongMat_->SetVector3f( "u_LightPosition", lObject_.transform.Position );
		Graphics::DrawMesh( *boxObject_.mesh, *phongMat_ );

		basicMat_->SetVector4f( "u_Color", lObject_.color );
		basicMat_->SetMatrix4f( "u_Model", lObject_.transform.TRS( ) );
		Graphics::DrawMesh( *lObject_.mesh, *basicMat_ );
	}
};

SET_ILLITHID_APPLICATION( TestApplication );

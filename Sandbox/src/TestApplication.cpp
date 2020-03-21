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
	std::unique_ptr<Material> basicMat_, phongMat_, colorMat_;;

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
		Shader basicShader( "Assets/Shaders/basic.shader" );
		basicMat_ = std::make_unique<Material>( basicShader );

		Shader phongShader( "Assets/Shaders/phong.shader" );
		phongMat_ = std::make_unique<Material>( phongShader );

		Shader colorShader( "Assets/Shaders/color.shader" );
		colorMat_ = std::make_unique<Material>( colorShader );

		boxObject_.mesh = StaticMesh::Load( "Assets/Models/cup.obj" );
		lObject_.mesh = StaticMesh::Load( "Assets/Models/box.obj" );

		cObject_.camera.SetPerspectiveProjection( PerspectiveProjection{ glm::radians( 60.0f ), Screen::Width( ) / static_cast<float_t>( Screen::Height( ) ), 0.1f, 100.0f } );
		cObject_.transform.Translate( glm::vec3( 0.0f, 0.0f, 1.0f ) );

		lObject_.transform.Scale( glm::vec3( 0.25f ) );
		lObject_.color = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );

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

			if (glm::abs( glm::degrees( rotation_.y ) ) > 70.0f)
			{
				deltaY = 0.0f;
				rotation_.y = glm::sign( glm::degrees( rotation_.y ) ) * glm::radians( 70.0f );
			}

			cObject_.transform.Rotate( glm::vec3( deltaY, deltaX, 0.0f ), itd::TransformationSpace::Camera );


			return false;
		} );

		dispatcher.Dispatch<itd::WindowResizedEvent>( [ this ] ( itd::WindowResizedEvent& evnt ) -> bool
		{
			cObject_.camera.SetPerspectiveProjection( PerspectiveProjection{ glm::radians( 60.0f ), Screen::Width( ) / static_cast<float_t>( Screen::Height( ) ), 0.1f, 100.0f } );
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

	}

	void DrawAxesLines( const AxesLines& axes, const glm::mat4& modelMatrix )
	{
		colorMat_->SetMatrix4f( "u_Model", modelMatrix );
		for (size_t i = 0; i < axes.lines.size( ); i++)
		{
			colorMat_->SetVector4f( "u_Color", axes.colors[ i ] );
			Graphics::DrawLineSegment( *axes.lines[ i ], *colorMat_ );
		}
	}


	// Inherited via Application
	virtual void Render( ) override
	{
		MoveCamera( );
		float_t angle = Time::Elapsed( ) * 0.5f;
		lObject_.transform.Position = glm::vec3( 2.0f * glm::cos( angle ), 0.0f, 2.0f * glm::sin( angle ) );

		cObject_.transform.Update( );
		boxObject_.transform.Update( );
		lObject_.transform.Update( );

		basicMat_->SetMatrix4f( "u_ViewProjection", cObject_.camera.CalculateViewProjection( cObject_.transform.InverseTRS( ) ) );
		phongMat_->SetMatrix4f( "u_ViewProjection", cObject_.camera.CalculateViewProjection( cObject_.transform.InverseTRS( ) ) );
		colorMat_->SetMatrix4f( "u_ViewProjection", cObject_.camera.CalculateViewProjection( cObject_.transform.InverseTRS( ) ) );

		phongMat_->SetMatrix4f( "u_Model", boxObject_.transform.TRS( ) );
		phongMat_->SetMatrix3f( "u_NormalMatrix", glm::mat3( glm::transpose( boxObject_.transform.InverseTRS( ) ) ) );
		phongMat_->SetVector4f( "u_Color", glm::vec4( 1.0f, 0.5f, 0.31f, 1.0f ) );
		phongMat_->SetFloat( "u_AmbientStrength", 0.1f );
		phongMat_->SetFloat( "u_SpecularStrength", 0.5f );
		phongMat_->SetVector4f( "u_LightColor", lObject_.color );
		phongMat_->SetVector3f( "u_LightPosition", lObject_.transform.Position );
		phongMat_->SetVector3f( "u_ViewPosition", cObject_.transform.Position );
		Graphics::DrawMesh( *boxObject_.mesh, *phongMat_ );

		basicMat_->SetVector4f( "u_Color", lObject_.color );
		basicMat_->SetMatrix4f( "u_Model", lObject_.transform.TRS( ) );
		Graphics::DrawMesh( *lObject_.mesh, *basicMat_ );

		DrawAxesLines( boxObject_.axes, boxObject_.transform.TRS( ) );
		DrawAxesLines( lObject_.axes, lObject_.transform.TRS( ) );
	}
};

SET_ILLITHID_APPLICATION( TestApplication );

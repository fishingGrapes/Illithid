#pragma once

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

using SceneManager = itd::SceneManager;
using Scene = itd::Scene;
using Resources = itd::Resources;

class TestApplication : public itd::Application
{

private:

	glm::vec2 mousePosition_;
	glm::vec3 rotation_;

	GameObject* barrel01_;
	GameObject* barrel02_;
	GameObject* barrel03_;

	GameObject* camera_;

	GameObject* dirLight_;
	GameObject* spotLight_;

	std::array<GameObject*, 4> pointLights_;


	std::vector<glm::vec3> pointLightPositions_ = {
			glm::vec3( 0.7f,  0.2f,  2.0f ),
			glm::vec3( -4.0f,  1.0f, -1.0f ),
			glm::vec3( 0.0f,  0.0f, -2.0f ),
				glm::vec3( 2.3f, -1.3f, 1.0f )
	};

	std::vector<glm::vec3> pointLightColors_ = {
		glm::vec3( 0.5f,  0.5f,  1.0f ),
		glm::vec3( 0.75f,  0.1f, 0.15f ),
		glm::vec3( 0.2f,  0.65f, 0.0f ),
			glm::vec3( 0.3f, 0.3f, 0.0f )
	};


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
		std::shared_ptr<Scene> phongScene = std::make_shared<Scene>( );


		std::shared_ptr<Texture2D> barrel_diffuse = Texture2D::Load( "Assets/Textures/barrel_diffuse.tex2D" );
		std::shared_ptr<Texture2D> barrel_specular = Texture2D::Load( "Assets/Textures/barrel_specular.tex2D" );

		Shader phongShader( "Assets/Shaders/phong.shader" );
		std::shared_ptr<Material> phongMat = std::make_shared<Material>( phongShader );
		phongMat->SetTexture( "u_Material.diffuse", barrel_diffuse );
		phongMat->SetTexture( "u_Material.specular", barrel_specular );
		phongMat->SetFloat( "u_Material.shininess", 32.0f );

		std::shared_ptr<StaticMesh> barrelMesh = StaticMesh::Load( "Assets/Models/barrel.obj" );

		barrel01_ = GameObject::Instantiate( "Box_01" );
		dptr<MeshRenderer> objRenderer_ = barrel01_->AddComponent<MeshRenderer>( );
		objRenderer_->Mesh = barrelMesh;
		objRenderer_->Material = phongMat;
		barrel01_->GetTransform( )->Translate( glm::vec3( 0.0f, 0.0f, 0.0f ) );
		barrel01_->GetTransform( )->Scale( glm::vec3( 0.25f, 0.25f, 0.25f ) );

		barrel02_ = GameObject::Instantiate( "Box_02" );
		objRenderer_ = barrel02_->AddComponent<MeshRenderer>( );
		objRenderer_->Mesh = barrelMesh;
		objRenderer_->Material = phongMat;
		barrel02_->GetTransform( )->Translate( glm::vec3( -1.0f, -1.0f, 0.0f ) );
		barrel02_->GetTransform( )->SetParent( barrel01_->GetTransform( ) );

		barrel03_ = GameObject::Instantiate( "Box_02" );
		objRenderer_ = barrel03_->AddComponent<MeshRenderer>( );
		objRenderer_->Mesh = barrelMesh;
		objRenderer_->Material = phongMat;
		barrel03_->GetTransform( )->Translate( glm::vec3( -1.0f, -1.0f, 0.0f ) );
		barrel03_->GetTransform( )->SetParent( barrel02_->GetTransform( ) );



		camera_ = GameObject::Instantiate( "Camera" );
		camera_->GetTransform( )->Translate( glm::vec3( 0.0f, 0.0f, 1.0f ) );
		auto cam = camera_->AddComponent<Camera>( );
		cam->SetPerspectiveProjection( PerspectiveProjection{ glm::radians( 60.0f ), Screen::Width( ) / static_cast<float_t>( Screen::Height( ) ), 0.1f, 100.0f } );
		Camera::SetAsPrimary( cam );

		CreateDirectionalLight( );
		CreatePointLights( );
		CreateSpotLight( );

		mousePosition_ = glm::vec2( Screen::Width( ) * 0.5f, Screen::Height( ) * 0.5f );

		phongScene->AddRootObject( barrel01_ );
		phongScene->AddRootObject( camera_ );
		phongScene->AddRootObject( dirLight_ );
		phongScene->AddRootObject( spotLight_ );

		for (size_t i = 0; i < pointLights_.size( ); i++)
		{
			phongScene->AddRootObject( pointLights_[ i ] );
		}

		SceneManager::LoadScene( phongScene );
		Graphics::SetSkybox( "Assets/Skyboxes/YokohamaNights.sbox" );
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

		if (Input::IsKeyDown( itd::KeyCode::ESCAPE ))
		{
			Application::Quit( );
		}

	}

	// Inherited via Application
	virtual void Update( ) override
	{
		barrel01_->GetTransform( )->Rotate( glm::vec3( 0.0f, 0.01f, 0.01f ), itd::TransformationSpace::Local );
		barrel02_->GetTransform( )->Rotate( glm::vec3( 0.0f, -0.05f, 0.0f ), itd::TransformationSpace::Local );
		barrel03_->GetTransform( )->Rotate( glm::vec3( 0.0f, 0.0f, 0.0f ), itd::TransformationSpace::World );

		MoveCamera( );

		spotLight_->GetTransform( )->Position = camera_->GetTransform( )->Position;
		spotLight_->GetTransform( )->Orientation = camera_->GetTransform( )->Orientation;
	}




private:
	void CreateDirectionalLight( )
	{
		dirLight_ = GameObject::Instantiate( "Dir_Light" );
		auto lightComp = dirLight_->AddComponent<Light>( itd::LightType::Directional );
		lightComp->Color = glm::vec3( 1.0f, 0.0f, 1.0f );


	}

	void CreatePointLights( )
	{
		for (size_t i = 0; i < pointLights_.size( ); ++i)
		{
			pointLights_[ i ] = GameObject::Instantiate( "Point_Light_" + std::to_string( i ) );

			auto lightComp = pointLights_[ i ]->AddComponent<Light>( itd::LightType::Point );
			lightComp->Color = glm::vec3( 1.0f, 0.682f, 0.259f );

			pointLights_[ i ]->GetTransform( )->Translate( pointLightPositions_[ i ] );
			pointLights_[ i ]->GetTransform( )->Scale( glm::vec3( 0.03125f ) );
		}
	}

	void CreateSpotLight( )
	{
		spotLight_ = GameObject::Instantiate( "Spot_Light" );
		/*auto lightComp = spotLight_->AddComponent<Light>( itd::LightType::Spot );
		lightComp->Color = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );*/

	}


};

SET_ILLITHID_APPLICATION( TestApplication );

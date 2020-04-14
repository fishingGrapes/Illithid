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

class TestApplication : public itd::Application
{

private:

	glm::vec2 mousePosition_;
	glm::vec3 rotation_;

	std::unique_ptr<GameObject> barrel_, camera_;
	std::unique_ptr<GameObject>  dirLight_, spotLight_;
	std::vector<std::unique_ptr<GameObject>> pointLights_;

	std::vector<glm::vec3> pointLightPositions_ = {
			glm::vec3( 0.7f,  0.2f,  2.0f ),
			glm::vec3( 2.3f, -3.3f, -4.0f ),
			glm::vec3( -4.0f,  2.0f, -12.0f ),
			glm::vec3( 0.0f,  0.0f, -3.0f )
	};

	ptr_ref<MeshRenderer> objRenderer_;

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
		ptr_ref<MeshRenderer> rend;
		{
			std::unique_ptr<GameObject> go = std::make_unique<GameObject>( );
			rend = go->AddComponent<MeshRenderer>( );
		}
		if (rend != nullptr)
		{
			IL_CORE_INFO("Still Alive" );
		}

		Shader phongShader( "Assets/Shaders/phong.shader" );
		std::shared_ptr<Material> phongMat_ = std::make_unique<Material>( phongShader );

		std::shared_ptr<Texture2D> barrel_diffuse = Texture2D::Load( "Assets/Textures/barrel_diffuse.tex2D" );
		std::shared_ptr<Texture2D> barrel_specular = Texture2D::Load( "Assets/Textures/barrel_specular.tex2D" );

		barrel_ = std::make_unique<GameObject>( );
		objRenderer_ = barrel_->AddComponent<MeshRenderer>( );
		objRenderer_->Mesh = StaticMesh::Load( "Assets/Models/box.obj" );
		objRenderer_->Material = phongMat_;
		objRenderer_->Material->SetTexture( "u_Material.diffuse", barrel_diffuse );
		objRenderer_->Material->SetTexture( "u_Material.specular", barrel_specular );
		objRenderer_->Material->SetFloat( "u_Material.shininess", 32.0f );
		barrel_->GetTransform( )->Translate( glm::vec3( 1.0f, 0.0f, 0.0f ) );

		camera_ = std::make_unique<GameObject>( );
		camera_->GetTransform( )->Translate( glm::vec3( 0.0f, 0.0f, 1.0f ) );
		auto cam = camera_->AddComponent<Camera>( );
		cam->SetPerspectiveProjection( PerspectiveProjection{ glm::radians( 60.0f ), Screen::Width( ) / static_cast<float_t>( Screen::Height( ) ), 0.1f, 100.0f } );
		Camera::SetAsPrimary( cam );

		CreateDirectionalLight( );
		CreatePointLight( );
		CreateSpotLight( );

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

		if (Input::IsKeyDown( itd::KeyCode::SPACE ))
		{
			barrel_->RemoveComponent<MeshRenderer>( );
			if (objRenderer_ == nullptr)
			{
				IL_TRACE( "Removed Mesh from barrel" );
			}
		}

	}

	// Inherited via Application
	virtual void Update( ) override
	{
		barrel_->GetTransform( )->Rotate( glm::vec3( 0, 0.01f, 0 ), itd::TransformationSpace::Local );

		MoveCamera( );
		spotLight_->GetTransform( )->Position = camera_->GetTransform( )->Position;
		spotLight_->GetTransform( )->Orientation = camera_->GetTransform( )->Orientation;


		if (objRenderer_ != nullptr)
		{
			objRenderer_->Material->SetVector3f( "u_ViewPosition", camera_->GetTransform( )->Position );
			objRenderer_->Material->SetVector3f( "u_SpotLight.position", spotLight_->GetTransform( )->Position );
			objRenderer_->Material->SetVector3f( "u_SpotLight.direction", spotLight_->GetTransform( )->Forward( ) );
		}
	}


	// Inherited via Application
	virtual void PreRender( ) override
	{
		if (objRenderer_ != nullptr)
		{
			barrel_->GetComponent<MeshRenderer>( )->Material->SetMatrix3f( "u_NormalMatrix", glm::mat3( glm::transpose( barrel_->GetTransform( )->InverseTRS( ) ) ) );
		}
	}

private:
	void CreateDirectionalLight( )
	{
		dirLight_ = std::make_unique<GameObject>( );
		auto lightComp = dirLight_->AddComponent<Light>( itd::LightType::Directional );
		lightComp->Color = glm::vec4( 0.5f, 1.0f, 0.125f, 1.0f );

		auto renderer = barrel_->GetComponent<MeshRenderer>( );
		renderer->Material->SetVector3f( "u_DirectionalLight.direction", glm::vec3( -0.2f, -1.0f, -0.3f ) );

		renderer->Material->SetVector3f( "u_DirectionalLight.ambient", 0.005f * lightComp->Color );
		renderer->Material->SetVector3f( "u_DirectionalLight.diffuse", 0.4f * lightComp->Color );
		renderer->Material->SetVector3f( "u_DirectionalLight.specular", 0.5f * lightComp->Color );
	}

	void CreatePointLight( )
	{
		Shader basicShader( "Assets/Shaders/color.shader" );
		std::shared_ptr<Material> basicMat_ = std::make_unique<Material>( basicShader );

		for (size_t i = 0; i < pointLightPositions_.size( ); ++i)
		{
			pointLights_.emplace_back( std::make_unique<GameObject>( ) );

			pointLights_[ i ]->GetTransform( )->Translate( pointLightPositions_[ i ] );
			pointLights_[ i ]->GetTransform( )->Scale( glm::vec3( 0.125f ) );

			auto lightComp = pointLights_[ i ]->AddComponent<Light>( itd::LightType::Point );
			lightComp->Color = glm::vec4( glm::linearRand( 0.0f, 1.0f ), glm::linearRand( 0.5f, 1.0f ), glm::linearRand( 0.0f, 1.0f ), 1.0f );

			auto renderer = barrel_->GetComponent<MeshRenderer>( );
			std::string uniform = "u_PointLights[" + std::to_string( i );

			renderer->Material->SetVector3f( ( uniform + std::string( "].position" ) ).c_str( ), pointLightPositions_[ i ] );

			renderer->Material->SetVector3f( ( uniform + std::string( "].ambient" ) ).c_str( ), 0.005f * lightComp->Color );
			renderer->Material->SetVector3f( ( uniform + std::string( "].diffuse" ) ).c_str( ), 0.8f * lightComp->Color );
			renderer->Material->SetVector3f( ( uniform + std::string( "].specular" ) ).c_str( ), 1.0f * lightComp->Color );

			renderer->Material->SetFloat( ( uniform + std::string( "].constant" ) ).c_str( ), 1.0f );
			renderer->Material->SetFloat( ( uniform + std::string( "].linear" ) ).c_str( ), 0.009f );
			renderer->Material->SetFloat( ( uniform + std::string( "].quadratic" ) ).c_str( ), 0.032f );

		}

	}

	void CreateSpotLight( )
	{
		spotLight_ = std::make_unique<GameObject>( );
		auto lightComp = spotLight_->AddComponent<Light>( itd::LightType::Spot );
		lightComp->Color = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );

		auto renderer = barrel_->GetComponent<MeshRenderer>( );

		renderer->Material->SetVector3f( "u_SpotLight.ambient", glm::vec3( 0.0f ) );
		renderer->Material->SetVector3f( "u_SpotLight.diffuse", glm::vec3( 1.0f ) );
		renderer->Material->SetVector3f( "u_SpotLight.specular", glm::vec3( 1.0f ) );

		renderer->Material->SetFloat( "u_SpotLight.constant", 1.0f );
		renderer->Material->SetFloat( "u_SpotLight.linear", 0.009f );
		renderer->Material->SetFloat( "u_SpotLight.quadratic", 0.032f );

		renderer->Material->SetFloat( "u_SpotLight.innerCutoff", glm::cos( glm::radians( 12.5f ) ) );
		renderer->Material->SetFloat( "u_SpotLight.outerCutoff", glm::cos( glm::radians( 15.0f ) ) );
	}

};

SET_ILLITHID_APPLICATION( TestApplication );

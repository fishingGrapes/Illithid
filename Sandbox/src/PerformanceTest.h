#pragma once

#include "Illithid.h"
#include "illithid/core/EntryPoint.h"

using Input = itd::Input;
using GameObject = itd::GameObject;
using Screen = itd::Screen;

using Camera = itd::Camera;
using Transform = itd::Transform;
using PerspectiveProjection = itd::PerspectiveProjection;

class TestApplication : public itd::Application
{

private:
	std::vector<ptr_ref<GameObject>> objects_;
	ptr_ref<GameObject> camera_;

public:

	TestApplication( )
	{
	}

	~TestApplication( )
	{
		for (size_t i = 0; i < objects_.size( ); i++)
		{
			GameObject::Destroy( objects_[ i ] );
		}
		GameObject::Destroy( camera_ );
	}

	// Inherited via Application
	virtual void Start( ) override
	{
		camera_ = GameObject::Instantiate( "Camera" );
		camera_->GetTransform( )->Translate( glm::vec3( 0.0f, 0.0f, 1.0f ) );
		auto cam = camera_->AddComponent<Camera>( );
		cam->SetPerspectiveProjection( PerspectiveProjection{ glm::radians( 60.0f ), Screen::Width( ) / static_cast<float_t>( Screen::Height( ) ), 0.1f, 100.0f } );
		Camera::SetAsPrimary( cam );
	}

	virtual void Shutdown( ) override
	{
		IL_TRACE( "Shutdown" );
	}

	virtual void OnEvent( itd::Event& event ) override
	{
		Application::OnEvent( event );

	}

	// Inherited via Application
	virtual void Update( ) override
	{

		if (Input::IsKeyDown( itd::KeyCode::Q ) || Input::IsKeyHeld( itd::KeyCode::Q ))
		{
			objects_.push_back( GameObject::Instantiate( "GameObject_" + std::to_string( objects_.size( ) ) ) );
		}

		if (Input::IsKeyDown( itd::KeyCode::E ) || Input::IsKeyHeld( itd::KeyCode::E ))
		{
			if (objects_.size( ) > 0)
			{
				GameObject::Destroy( objects_.back( ) );
				objects_.pop_back( );
			}
		}
	}


	// Inherited via Application
	virtual void PreRender( ) override
	{
	}

};

SET_ILLITHID_APPLICATION( TestApplication );

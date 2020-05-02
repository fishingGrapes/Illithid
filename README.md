# Illithid Game Framework
A simple game framework I am making for learning purposes. It has an OpenGL renderer core and uses a Entity-Component architecture for interacting. This is a highly WIP.

![engineloop](https://github.com/fishingGrapes/Illithid/blob/master/images/engine.gif)

### Features
- [x] Logging Facility
- [x] Entity-Component Architecture
- [x] Basic Input Handling
- [x] Object loading and Mesh Rendering
- [x] Blinn-Phong Shading
- [ ] Physically based Rendering
- [x] Forward lighting pass
- [x] Skyboxes
- [ ] Normal Mapping
- [ ] Shadow mapping
- [ ] Post-Processing
- [x] Scene Hierarchy
- [ ] Lua Scripting

## Entity-Component Architecture
An implementation of Entity-Component architecture inspired by Unity3D and Unreal Engine.

#### Features
* A Entity-Componenet framework with a simple API, where GameObjects are composed of components.
* Fast iterations through components using a custom allocator which ensures that components of a given type are always tightly-packed together.
* Safety Measures to avoid adding duplicate components to GameObjects, removing or accessing non-existent components from GameObjects.
* Automatic deallocation of components and it's resources on GameObject destruction. This means, No need to worry about components once added.
* The framework also provides methods to remove a component on-demand from a GameObject.

![growinglistallocator](https://github.com/fishingGrapes/Illithid/blob/master/images/growinglistallocator.png)

I needed to iterate through a collection of components(similar to Unity3D) every frame. There are game objects which are composed of components and these components contain behaviour like Transform, Camera, Light, Mesh Renderer and so on, which need to be updated every frame. I could not afford to iterate through components at different places in memory. So I wrote this allocactor which is nothing but a list of fixed-size memory blocks and actively keeps the objects tightly packed on allocation and deallocation, so as to ensure the best cache locality and fast iterations.

## Usage
Here is a quick overview of how to use the framework API.

```c++
#pragma once

//Include this header to get access to the framework
#include "Illithid.h"
#include "illithid/core/EntryPoint.h"

//Inherit publicly from application to use the API
class TestApplication : public Application
{

private:
	GameObject* barrel01_;
	GameObject* barrel02_;
	GameObject* barrel03_;

	GameObject* cameraObject_;
	GameObject* dirLight_;


public:
	TestApplication( )
	{
	}

	~TestApplication( )
	{
	}

	// Initialization code goes here
	virtual void Start( ) override
	{
		//Create a new Scene
		std::shared_ptr<Scene> phongScene = std::make_shared<Scene>( );

		//Load Textures from files
		//.tex2D is a type which contains meta data about the actual texture
		std::shared_ptr<Texture2D> barrel_diffuse = Texture2D::Load( "Assets/Textures/barrel_diffuse.tex2D" );
		std::shared_ptr<Texture2D> barrel_specular = Texture2D::Load( "Assets/Textures/barrel_specular.tex2D" );

		//Create the material
		Shader phongShader( "Assets/Shaders/phong.shader" );
		std::shared_ptr<Material> phongMat = std::make_shared<Material>( phongShader );
		phongMat->SetTexture( "u_Material.diffuse", barrel_diffuse );
		phongMat->SetTexture( "u_Material.specular", barrel_specular );
		phongMat->SetFloat( "u_Material.shininess", 32.0f );

		//Load the mesh
		std::shared_ptr<StaticMesh> barrelMesh = StaticMesh::Load( "Assets/Models/barrel.obj" );

		//Start Creating game objects
		barrel01_ = GameObject::Instantiate( "Barrel_01" );
		dptr<MeshRenderer> barrelRenderer01 = barrel01_->AddComponent<MeshRenderer>( );
		barrelRenderer01->Mesh = barrelMesh;
		barrelRenderer01->Material = phongMat;
		barrel01_->GetTransform( )->Translate( glm::vec3( 0.0f, 0.0f, 0.0f ) );
		barrel01_->GetTransform( )->Scale( glm::vec3( 0.25f, 0.25f, 0.25f ) );

		barrel02_ = GameObject::Instantiate( "Barrel_02" );
		dptr<MeshRenderer>  barrelRenderer02 = barrel02_->AddComponent<MeshRenderer>( );
		barrelRenderer02->Mesh = barrelMesh;
		barrelRenderer02->Material = phongMat;
		barrel02_->GetTransform( )->Translate( glm::vec3( -1.0f, -1.0f, 0.0f ) );
		
		//Setting the parent and creating scene hierarchy
		//Barrel_01 is the parent of Barrel_02
		barrel02_->GetTransform( )->SetParent( barrel01_->GetTransform( ) );

		barrel03_ = GameObject::Instantiate( "Barrel_03" );
		dptr<MeshRenderer>  barrelRenderer03 = barrel03_->AddComponent<MeshRenderer>( );
		barrelRenderer03->Mesh = barrelMesh;
		barrelRenderer03->Material = phongMat;
		barrel03_->GetTransform( )->Translate( glm::vec3( -1.0f, -1.0f, 0.0f ) );
		
		//Barrel_02 is the parent of Barrel_03
		barrel03_->GetTransform( )->SetParent( barrel02_->GetTransform( ) );

		cameraObject_ = GameObject::Instantiate( "Camera" );
		cameraObject_->GetTransform( )->Translate( glm::vec3( 0.0f, 0.0f, 1.0f ) );
		dptr<Camera> camera = cameraObject_->AddComponent<Camera>( );

		//Set Camera projection
		camera->SetPerspectiveProjection( PerspectiveProjection{ glm::radians( 60.0f ), Screen::Width( ) / static_cast<float_t>( Screen::Height( ) ), 0.1f, 100.0f } );
		Camera::SetAsPrimary( camera );


		//Create the Scene Hierarchy
		phongScene->AddRootObject( barrel01_ );
		phongScene->AddRootObject( cameraObject_ );
		phongScene->AddRootObject( dirLight_ );
		phongScene->AddRootObject( spotLight_ );

		for (size_t i = 0; i < pointLights_.size( ); i++)
		{
			phongScene->AddRootObject( pointLights_[ i ] );
		}

		//Load the Scene
		SceneManager::LoadScene( phongScene );

		//Load the skyboox from file
		//.sbox is a type whcih contains meta data about the skybox textures
		Graphics::SetSkybox( "Assets/Skyboxes/YokohamaNights.sbox" );
	}


	//Callback for input events
	virtual void OnEvent( Event& event ) override
	{
		//Must call this, otherwise applicatiion events won't be processed 
		Application::OnEvent( event );

		EventDispatcher dispatcher( event );
		dispatcher.Dispatch<WindowResizedEvent>( [ this ] ( WindowResizedEvent& evnt ) -> bool
		{
			cameraObject_->GetComponent<Camera>( )->SetPerspectiveProjection( PerspectiveProjection{ glm::radians( 60.0f ), Screen::Width( ) / static_cast<float_t>( Screen::Height( ) ), 0.1f, 100.0f } );
			return false;
		} );
	}

	// Called every frame
	virtual void Update( ) override
	{
		//Rotating the barrels
		barrel01_->GetTransform( )->Rotate( glm::vec3( 0.0f, 0.01f, 0.01f ), TransformationSpace::Local );
		barrel02_->GetTransform( )->Rotate( glm::vec3( 0.0f, -0.05f, 0.0f ), TransformationSpace::Local );
		barrel03_->GetTransform( )->Rotate( glm::vec3( 0.0f, 0.0f, 0.0f ), TransformationSpace::World );

		//Input API
		if (Input::IsKeyDown( KeyCode::W ) || Input::IsKeyHeld( KeyCode::W ))
		{
			cameraObject_->GetTransform( )->Translate( Time::Delta( ) * cameraObject_->GetTransform( )->Forward( ) );
		}

		if (Input::IsKeyDown( KeyCode::S ) || Input::IsKeyHeld( KeyCode::S ))
		{
			cameraObject_->GetTransform( )->Translate( Time::Delta( ) * cameraObject_->GetTransform( )->Forward( ) * -1.0f );
		}

		if (Input::IsKeyDown( KeyCode::A ) || Input::IsKeyHeld( KeyCode::A ))
		{
			cameraObject_->GetTransform( )->Translate( Time::Delta( ) * cameraObject_->GetTransform( )->Right( ) * -1.0f );
		}

		if (Input::IsKeyDown( KeyCode::D ) || Input::IsKeyHeld( KeyCode::D ))
		{
			cameraObject_->GetTransform( )->Translate( Time::Delta( ) * cameraObject_->GetTransform( )->Right( ) );
		}

		if (Input::IsKeyDown( KeyCode::ESCAPE ))
		{
			Application::Quit( );
		}
	}

	virtual void Shutdown( ) override
	{
		//Logging facility
		IL_TRACE( "Shutdown" );
		
		//Destroying a game object
		//Everything in the Scene heirarchy is automatically destroy on Scene unloading
		GameObject::Destroy( dirLight_ );
	}

};

//VERY IMPORTANT: SET THE ENTRY POINT OF THE APPLICATION
SET_ILLITHID_APPLICATION( TestApplication );
```

## References
* [Game Engine series by Yan Chernikov](https://www.youtube.com/watch?v=JxIZbV_XjAs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT)
* [Learn OpenGL website by Joey de Vries](https://learnopengl.com/)
* [OpenGL Series by ThinMatrix](https://www.youtube.com/playlist?list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP)

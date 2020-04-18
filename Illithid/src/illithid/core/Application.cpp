#include "ilpch.h"
#include "Application.h"

#include "Log.h"
#include "Time.h"
#include "Screen.h"
#include "Window.h"
#include "Macros.h"
#include "FileSystem.h"
#include "Input.h"
#include "illithid/renderer/Graphics.h"
#include "Resources.h"
#include "illithid/imgui/Gui.h"

#include "illithid/components/ComponentRegistry.h"
#include "illithid/components/Transform.h"
#include "illithid/components/Camera.h"
#include "illithid/components/AxesGizmo.h"
#include "illithid/components/Light.h"
#include "illithid/components/MeshRenderer.h"

#include "illithid/utils/FrameRateCounter.h"

namespace itd
{
	float_t Time::elapsed_ = 0.0f;
	float_t Time::delta_ = 0.0f;

	int32_t Screen::width_ = 0;
	int32_t Screen::height_ = 0;

	bool Application::isRunning_ = true;

	Application::Application( )
	{
		this->Initialize( );
	}

	Application::~Application( )
	{
		this->Destroy( );
	}

	void Application::Run( )
	{
		using TimePoint = std::chrono::steady_clock::time_point;

		TimePoint startTime = std::chrono::high_resolution_clock::now( );
		TimePoint previousTime = startTime;
		TimePoint currentTime;


		ComponentRegistry::Start( );
		this->Start( );
		Graphics::EnableCapabality( Capability::Cap_DepthTest );

		while (isRunning_)
		{
			currentTime = std::chrono::high_resolution_clock::now( );
			Time::delta_ = ( std::chrono::duration_cast<std::chrono::microseconds>( currentTime - previousTime ).count( ) ) / 1000000.0f;
			Time::elapsed_ = ( std::chrono::duration_cast<std::chrono::microseconds>( currentTime - startTime ).count( ) ) / 1000000.0f;

			fpsCounter_->Update( Time::delta_ );
			window_->SetTitle( "GL Framerate: " + std::to_string( fpsCounter_->GetSmoothedFrameRate( ) ) + " FPS" );

			ComponentRegistry::Update( );
			this->Update( );

			ComponentRegistry::PreRender( );
			this->PreRender( );

			Graphics::ClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
			Graphics::Clear( BufferBit::BB_Color | BufferBit::BB_Depth );

			ComponentRegistry::Render( );

			Gui::Begin( );
			//GUI Layer Code Here
			Gui::End( window_ );

			ComponentRegistry::PostRender( );

			window_->SwapBuffers( );
			window_->PollEvents( );

			previousTime = currentTime;
		}

		this->Shutdown( );
	}

	inline int32_t Application::FrameRate( ) const
	{
		return fpsCounter_->GetSmoothedFrameRate( );
	}

	void Application::OnEvent( Event& event )
	{
		EventDispatcher dispatcher( event );
		dispatcher.Dispatch<WindowClosedEvent>( BIND_EVENT_FUNCTION( Application::OnWindowClosed, this ) );
		dispatcher.Dispatch<WindowResizedEvent>( BIND_EVENT_FUNCTION( Application::OnWindowResized, this ) );

		Input::ProcessEvent( event );
	}


	void Application::Initialize( )
	{
		//Change this to take a log-level
		Log::Initialize( );

		FileSystem::Initialize( );

		window_ = std::make_unique<Window>( WindowProperties( "GL", 1280, 720 ) );
		window_->SetEventListener( BIND_EVENT_FUNCTION( Application::OnEvent, this ) );

		fpsCounter_ = std::make_unique<FrameRateCounter>( );

		Screen::width_ = window_->Width( );
		Screen::height_ = window_->Height( );

		Graphics::Initialize( window_ );
		Input::Initialize( );

		Resources::Initialize( );
		Gui::Initialize( window_ );

		this->RegisterEngineComponents( );
	}

	void Application::Destroy( )
	{
		ComponentRegistry::Destroy( );
		Input::Destroy( );
		Gui::Destroy( );
	}

	void Application::RegisterEngineComponents( )
	{
		ComponentRegistry::Register<Transform>( false, true, false, false, false );
		ComponentRegistry::Register<AxesGizmo>( true, false, true, true, false );
		ComponentRegistry::Register<Camera>( false, false, false, false, false );
		ComponentRegistry::Register<Light>( true, false, true, true, false );
		ComponentRegistry::Register<MeshRenderer>( false, false, true, true, false );

		IL_CORE_ERROR( "{0} {1} {2} {3} {4} ", Transform::ID, AxesGizmo::ID, Camera::ID, Light::ID, MeshRenderer::ID );
	}

	bool Application::OnWindowClosed( WindowClosedEvent& event )
	{
		IL_CORE_INFO( "Closing Window.." );

		isRunning_ = false;
		return true;
	}
	bool Application::OnWindowResized( WindowResizedEvent& event )
	{
		Screen::width_ = event.Width( );
		Screen::height_ = event.Height( );

		return false;
	}

}
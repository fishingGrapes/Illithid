#include "ilpch.h"
#include "Application.h"

#include "Log.h"
#include "Window.h"
#include "illithid/io/Input.h"
#include "Macros.h"
#include "Time.h"

namespace itd
{

	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::steady_clock::time_point;

	double_t Time::frameDeltaTime_ = 0.0;
	double_t Time::interpolationFactor_ = 0.0;
	uint32_t Time::frameCount_ = -1;
	uint32_t Time::timeStepCount_ = -1;

	Application::Application( ) :
		isRunning_( true )
	{
		this->Initialize( );
	}

	Application::~Application( )
	{
		this->Destroy( );
	}

	void Application::Run( )
	{
		this->Start( );

		using namespace std::chrono_literals;

		int64_t timeStepNanos = static_cast<uint64_t>( Time::FixedTimeStep * ONE_NANO_SECOND );
		const std::chrono::nanoseconds timeStep( timeStepNanos );

		int64_t maxDeltaNanos = static_cast<uint64_t>( Time::MaximumDeltaTime * ONE_NANO_SECOND );
		const std::chrono::nanoseconds maxDeltaTime( maxDeltaNanos );

		auto previousTime = Clock::now( );
		std::chrono::nanoseconds accumulator( 0ns );

		Time::frameCount_ = -1;
		Time::timeStepCount_ = -1;
		uint32_t iteration = 0;

		while (isRunning_)
		{
			window_->PollEvents( );

			TimePoint currentTime = Clock::now( );
			auto deltaTime = std::min( ( currentTime - previousTime ), maxDeltaTime );
			previousTime = currentTime;

			++Time::frameCount_;
			iteration = 0;

			Time::frameDeltaTime_ = std::chrono::duration<double_t>( deltaTime ).count( );

			accumulator += std::chrono::duration_cast<std::chrono::nanoseconds>( deltaTime );
			while (accumulator >= timeStep)
			{
				++Time::timeStepCount_;
				++iteration;

				// Initialize ConditionalVariable = (currentTime - deltaTime) + timeStep
				TimePoint maxTimeStamp = ( currentTime - deltaTime ) + ( timeStep * iteration );

				// Is the Buffer Empty?
				while (Input::IsEventAvailable( ))
				{
					BufferedEvent& bufferedEvnt = Input::BufferEvent( );

					// Is the Buffered Event eligibe for processing this Tick?
					if (bufferedEvnt.TimeStamp <= maxTimeStamp)
					{
						std::shared_ptr <Event> evnt = bufferedEvnt.Evnt;
						// Pass the Event to input for generating Input States.
						Input::ProcessBufferedInput( evnt );
					}
					else
					{
						break;
					}
				}

				//Process bufffered events here
				this->FixedTick( Time::FixedTimeStep );
				accumulator -= timeStep;
			}

			Time::interpolationFactor_ = accumulator.count( ) / (double_t) timeStep.count( );

			this->Tick( Time::interpolationFactor_, Time::frameDeltaTime_ );
			this->LateTick( Time::interpolationFactor_, Time::frameDeltaTime_ );

			window_->SwapBuffers( );
		}

		this->Shutdown( );
	}

	void Application::OnEvent( std::shared_ptr<Event> event )
	{
		EventDispatcher dispatcher( event );
		dispatcher.Dispatch<WindowClosedEvent>( BIND_EVENT_FUNCTION( Application::OnWindowClosed, this ) );

		BufferedEvent bufferedEvent( event, Clock::now( ) );
		Input::RecordEvent( bufferedEvent );
	}

	void Application::Initialize( )
	{
		//Change this to take a log-level
		Log::Initialize( );
		IL_CORE_INFO( "Logger Initialized." );

		window_ = std::make_unique<Window>( WindowProperties( ) );
		window_->SetEventListener( BIND_EVENT_FUNCTION( Application::OnEvent, this ) );
		IL_CORE_INFO( "Window Initialized." );

		Input::Initialize( );
		IL_CORE_INFO( "Input Initialized." );

	}

	void Application::Destroy( )
	{
		Input::Destroy( );
	}

	bool Application::OnWindowClosed( WindowClosedEvent& event )
	{
		IL_CORE_INFO( "Window Closed." );

		isRunning_ = false;
		return true;
	}
}
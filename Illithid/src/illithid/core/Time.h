#pragma once

#include <chrono>

namespace itd
{
	constexpr double_t SIMULATION_RATE = 30.0;
	constexpr double_t MAX_DELTA_TIME = 0.25;
	constexpr uint64_t ONE_NANO_SECOND = 1000000000ll;

	struct Time
	{
		friend class Application;
	
	public:

		constexpr static double_t FixedTimeStep = ( 1.0 / SIMULATION_RATE );
		constexpr static double_t MaximumDeltaTime = MAX_DELTA_TIME;

		inline static const double_t FrameDeltaTime( )
		{
			return frameDeltaTime_;
		}

		inline static const double_t InterpolationFactor( )
		{
			return interpolationFactor_;
		}

		inline static const uint32_t FrameCount( )
		{
			return frameCount_;
		}

		inline static const uint32_t TimeStepCount( )
		{
			return timeStepCount_;
		}

	private:
		static double_t frameDeltaTime_;
		static double_t interpolationFactor_;
		static uint32_t frameCount_;
		static uint32_t timeStepCount_;
	};

}

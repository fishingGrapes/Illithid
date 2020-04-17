#pragma once
#include <deque>

class FrameRateCounter
{
public:
	FrameRateCounter( )
		:frameTimeSum_( 0.0f )
	{
		frameTimes_.resize( FRAME_TIME_SIZE, 0.0f );
	}

	~FrameRateCounter( )
	{
		frameTimes_.clear( );
	}

	void Update( float_t dt )
	{
		while (frameTimes_.size( ) >= FRAME_TIME_SIZE)
		{
			frameTimeSum_ -= frameTimes_.front( );
			frameTimes_.pop_front( );
		}

		while (frameTimes_.size( ) < FRAME_TIME_SIZE)
		{
			frameTimeSum_ += dt;
			frameTimes_.push_back( dt );
		}
	}

	inline int32_t GetSmoothedFrameRate( ) const
	{
		return static_cast<int32_t>( static_cast<float_t>( FRAME_TIME_SIZE ) / frameTimeSum_ );
	}

private:
	const int32_t FRAME_TIME_SIZE = 100;
	std::deque<float_t> frameTimes_;
	float_t frameTimeSum_;
};
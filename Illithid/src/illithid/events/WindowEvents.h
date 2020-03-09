#pragma once

#include <sstream>
#include "Event.h"

namespace itd
{
	class WindowResizedEvent : public Event
	{
	public:
		explicit WindowResizedEvent( uint32_t width, uint32_t height ) : width_( width ), height_( height )
		{

		}

		inline uint32_t Width( ) const
		{
			return width_;
		}

		inline uint32_t Height( ) const
		{
			return height_;
		}

		virtual std::string ToString( ) const override
		{
			std::stringstream sstream;
			sstream << "WindowResizedEvent : (" << width_ << ", " << height_ << ")";
			return sstream.str( );
		}

		EVENT_CLASS_TYPE( WindowResized )
	private:
		uint32_t width_, height_;
	};

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent( ) = default;

		virtual std::string ToString( ) const override
		{
			std::stringstream sstream;
			sstream << "WindowClosedEvent";
			return sstream.str( );
		}

		EVENT_CLASS_TYPE( WindowClosed )
	};
}
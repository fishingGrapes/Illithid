#pragma once

#include <sstream>
#include "Event.h"


namespace itd
{
	class MouseMovedEvent : public Event
	{
	public:
		explicit MouseMovedEvent( float_t x, float_t y )
			:mouseX_( x ), mouseY_( y )
		{

		}

		inline float_t X( ) const
		{
			return mouseX_;
		}

		inline float_t Y( ) const
		{
			return mouseY_;
		}

		virtual std::string ToString( ) const override
		{
			std::stringstream sstream;
			sstream << "MouseMovedEvent: (" << mouseX_ << ", " << mouseY_ << ")";
			return sstream.str( );
		}

		EVENT_CLASS_TYPE( MouseMoved )
	private:
		float_t mouseX_, mouseY_;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		explicit MouseScrolledEvent( float_t xOffset, float_t yOffset ) :scrollX_( xOffset ), scrollY_( yOffset )
		{

		}

		inline float_t ScrollX( ) const
		{
			return scrollX_;
		}

		inline float_t ScrollY( ) const
		{
			return scrollY_;
		}

		virtual std::string ToString( ) const override
		{
			std::stringstream sstream;
			sstream << "MouseScrolledEvent: (" << scrollX_ << ", " << scrollY_ << ")";
			return sstream.str( );
		}

		EVENT_CLASS_TYPE( MouseScrolled )
	private:
		float_t scrollX_, scrollY_;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline uint8_t Button( ) const
		{
			return button_;
		}
	protected:
		explicit MouseButtonEvent( uint8_t button ) : button_( button )
		{

		}

		uint8_t button_;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		explicit MouseButtonPressedEvent( uint8_t button ) : MouseButtonEvent( button )
		{

		}

		virtual std::string ToString( ) const override
		{
			std::stringstream sstream;
			sstream << "MouseButtonPressedEvent: " << button_;
			return sstream.str( );
		}

		EVENT_CLASS_TYPE( MouseButtonPressed )
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		explicit MouseButtonReleasedEvent( uint8_t button ) : MouseButtonEvent( button )
		{

		}

		virtual std::string ToString( ) const override
		{
			std::stringstream sstream;
			sstream << "MouseButtonReleasedEvent: " << button_;
			return sstream.str( );
		}

		EVENT_CLASS_TYPE( MouseButtonReleased )
	};
}
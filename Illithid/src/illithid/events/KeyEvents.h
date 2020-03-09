#pragma once

#include <sstream>
#include "Event.h"

namespace itd
{
	class KeyEvent : public Event
	{
	public:
		inline int KeyCode( ) const
		{
			return keyCode_;
		}

	protected:

		explicit KeyEvent( uint32_t keyCode ) :keyCode_( keyCode )
		{

		}

		uint32_t keyCode_;
	};


	class KeyPressedEvent : public KeyEvent
	{
	public:
		explicit KeyPressedEvent( uint32_t keyCode ) : KeyEvent( keyCode ), repeatCount_( 0 )
		{

		}

		inline uint32_t RepeatCount( ) const
		{
			return repeatCount_;
		}

		virtual std::string ToString( ) const override
		{
			std::stringstream sstream;
			sstream << "KeyPressedEvent: " << keyCode_ << " (" << repeatCount_ << " repeats)";
			return sstream.str( );
		}

		EVENT_CLASS_TYPE( KeyPressed )
	private:
		uint32_t repeatCount_;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		explicit KeyReleasedEvent( uint32_t keyCode ) :KeyEvent( keyCode )
		{

		}

		virtual std::string ToString( ) const override
		{
			std::stringstream sstream;
			sstream << "KeyReleasedEvent: " << keyCode_;
			return sstream.str( );
		}

		EVENT_CLASS_TYPE( KeyReleased )
	};
}

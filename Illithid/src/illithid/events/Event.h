#pragma once

#include <functional>
#include <string>
#include <ostream>

namespace itd
{
	enum class EventType
	{
		None = 0,
		WindowClosed, WindowResized, WindowFocused, WindowFocusLost, WindowMoved,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

#define EVENT_CLASS_TYPE(type) static EventType Type() { return EventType::##type; }\
							   virtual EventType GetType() const override { return Type(); }

	class Event
	{
		friend class EventDispatcher;

	public:
		virtual EventType GetType( ) const = 0;
		virtual std::string ToString( ) const = 0;

	protected:
		bool handled_;
	};


	class EventDispatcher
	{
		template<typename T>
		using EventFunction = std::function<bool( T& )>;

	public:
		explicit EventDispatcher( Event& event ) : event_( event )
		{

		}

		template<typename T>
		bool Dispatch( EventFunction<T> func )
		{
			if (event_.GetType( ) == T::Type( ))
			{
				event_.handled_ = func( *( (T*) &event_ ) );
				return true;
			}
			return false;
		}

	private:
		Event& event_;
	};

	inline std::ostream& operator<< ( std::ostream& os, const Event& evnt )
	{
		return os << evnt.ToString( );
	}
}

#include "ilpch.h"
#include "Input.h"

#include "illithid/core/Macros.h"
#include "illithid/core/Log.h"

namespace itd
{
	Input* Input::instance_ = nullptr;

	void Input::Initialize( )
	{
		if (instance_ == nullptr)
		{
			instance_ = new Input( );
		}
	}

	void Input::Destroy( )
	{
		if (instance_ != nullptr)
		{
			delete instance_;
			instance_ = nullptr;
		}
	}

	Input::Input( )
	{
		currentInputState_.reset( );
		prevInputState_.reset( );

		mousePositionState_ = MousePositionState{ glm::vec2( 0.0f ), glm::vec2( 0.0f ) };
		mouseScrollState_ = MouseScrollState{ glm::vec2( 0.0f ),  false };
	}

	bool Input::IsAnyKeyDown( )
	{
		return instance_->currentInputState_.any( );
	}

	bool Input::IsKeyDown( KeyCode keyCode )
	{
		uint16_t index = static_cast<uint16_t>( keyCode );
		if (instance_->prevInputState_[ index ] == false && instance_->currentInputState_[ index ] == true)
		{
			instance_->prevInputState_[ index ] = instance_->currentInputState_[ index ] = true;
			return true;
		}
		return false;
	}

	bool Input::IsKeyUp( KeyCode keyCode )
	{
		uint16_t index = static_cast<uint16_t>( keyCode );
		if (instance_->prevInputState_[ index ] == true && instance_->currentInputState_[ index ] == false)
		{
			return true;
		}
		return false;
	}

	bool Input::IsKeyHeld( KeyCode keyCode )
	{
		uint16_t index = static_cast<uint16_t>( keyCode );
		return ( instance_->prevInputState_[ index ] == true && instance_->currentInputState_[ index ] == true );
	}

	bool Input::IsMouseButtonDown( int16_t mouse )
	{
		if (instance_->prevInputState_[ mouse ] == false && instance_->currentInputState_[ mouse ] == true)
		{
			instance_->prevInputState_[ mouse ] = instance_->currentInputState_[ mouse ] = true;
			return true;
		}
		return false;
	}

	bool Input::IsMouseButtonUp( int16_t mouse )
	{
		if (instance_->prevInputState_[ mouse ] == true && instance_->currentInputState_[ mouse ] == false)
		{
			return true;
		}
		return false;
	}

	bool Input::IsMouseButtonHeld( int16_t mouse )
	{
		return ( instance_->prevInputState_[ mouse ] == true && instance_->currentInputState_[ mouse ] == true );
	}



	glm::vec2 Input::GetMouseScrollDelta( )
	{
		if (instance_->mouseScrollState_.ScrolledSinceLastPoll)
		{
			instance_->mouseScrollState_.ScrolledSinceLastPoll = false;
			return instance_->mouseScrollState_.Value;
		}

		return glm::vec2( 0.0f );
	}


	void Input::ProcessEvent( Event& event )
	{
		EventDispatcher dispatcher( event );

		dispatcher.Dispatch<KeyPressedEvent>( BIND_EVENT_FUNCTION( Input::OnKeyPressed, instance_ ) );
		dispatcher.Dispatch<KeyReleasedEvent>( BIND_EVENT_FUNCTION( Input::OnKeyReleased, instance_ ) );
		dispatcher.Dispatch<MouseButtonPressedEvent>( BIND_EVENT_FUNCTION( Input::OnMouseButtonPressed, instance_ ) );
		dispatcher.Dispatch<MouseButtonReleasedEvent>( BIND_EVENT_FUNCTION( Input::OnMouseButtonReleased, instance_ ) );
		dispatcher.Dispatch<MouseMovedEvent>( BIND_EVENT_FUNCTION( Input::OnMouseMovedEvent, instance_ ) );
		dispatcher.Dispatch<MouseScrolledEvent>( BIND_EVENT_FUNCTION( Input::OnMouseScrolledEvent, instance_ ) );

	}

	bool Input::OnKeyPressed( KeyPressedEvent& event )
	{
		prevInputState_[ event.KeyCode( ) ] = currentInputState_[ event.KeyCode( ) ];
		currentInputState_[ event.KeyCode( ) ] = true;
		return false;
	}

	bool Input::OnKeyReleased( KeyReleasedEvent& event )
	{
		prevInputState_[ event.KeyCode( ) ] = true;
		currentInputState_[ event.KeyCode( ) ] = false;
		return false;
	}

	bool Input::OnMouseButtonPressed( MouseButtonPressedEvent& event )
	{
		prevInputState_[ event.Button( ) ] = currentInputState_[ event.Button( ) ];
		currentInputState_[ event.Button( ) ] = true;
		return false;
	}

	bool Input::OnMouseButtonReleased( MouseButtonReleasedEvent& event )
	{
		prevInputState_[ event.Button( ) ] = true;
		currentInputState_[ event.Button( ) ] = false;
		return false;
	}

	bool Input::OnMouseMovedEvent( MouseMovedEvent& event )
	{
		instance_->mousePositionState_.Previous = instance_->mousePositionState_.Current;
		instance_->mousePositionState_.Current = glm::vec2( event.X( ), event.Y( ) );

		return false;
	}

	bool Input::OnMouseScrolledEvent( MouseScrolledEvent& event )
	{
		instance_->mouseScrollState_.Value = glm::vec2( event.ScrollX( ), event.ScrollY( ) );
		instance_->mouseScrollState_.ScrolledSinceLastPoll = true;

		return false;
	}
}

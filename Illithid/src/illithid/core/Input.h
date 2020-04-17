#pragma once

#include <bitset>
#include <chrono>
#include <memory>
#include <queue>

#include "illithid/events/KeyEvents.h"
#include "illithid/events/MouseEvents.h"
#include "KeyCodes.h"
#include "glm/glm.hpp"

namespace itd
{
	constexpr uint16_t MAX_INPUT_CODES = 400;

	class Input
	{

	public:

		Input( const Input& other ) = delete;
		Input( Input&& other ) = delete;
		Input& operator=( const Input& ) = delete;
		Input& operator=( Input&& ) = delete;


		static bool IsAnyKeyDown( );

		static bool IsKeyDown( KeyCode keyCode );
		static bool IsKeyUp( KeyCode keyCode );
		static bool IsKeyHeld( KeyCode keyCode );

		static bool IsMouseButtonDown( int16_t mouse );
		static bool IsMouseButtonUp( int16_t mouse );
		static bool IsMouseButtonHeld( int16_t mouse );


		static inline glm::vec2 GetMousePosition( )
		{
			return instance_->mousePositionState_.Current;
		}

		static glm::vec2 GetMouseScrollDelta( );

	private:

		bool OnKeyPressed( KeyPressedEvent& event );
		bool OnKeyReleased( KeyReleasedEvent& event );

		bool OnMouseButtonPressed( MouseButtonPressedEvent& event );
		bool OnMouseButtonReleased( MouseButtonReleasedEvent& event );

		bool OnMouseMovedEvent( MouseMovedEvent& event );
		bool OnMouseScrolledEvent( MouseScrolledEvent& event );

		std::bitset<MAX_INPUT_CODES> currentInputState_;
		std::bitset<MAX_INPUT_CODES> prevInputState_;

		struct MousePositionState
		{
			glm::vec2 Previous;
			glm::vec2 Current;
		} mousePositionState_;

		struct MouseScrollState
		{
			glm::vec2 Value;
			bool ScrolledSinceLastPoll;

		} mouseScrollState_;

		friend class Application;

		Input( );
		static Input* instance_;

		static void Initialize( );
		static void Destroy( );

		static void ProcessEvent( Event& event );
	};
}

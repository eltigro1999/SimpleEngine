#pragma once

namespace SimpleEngine {

	enum class EventType {
		WindowResize=0,
		WindowClose,

		KeyPressed,
		KeyReleased,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,

		EventsCount
	};

	struct Event{
		unsigned int width;
		unsigned int height;
	};
}
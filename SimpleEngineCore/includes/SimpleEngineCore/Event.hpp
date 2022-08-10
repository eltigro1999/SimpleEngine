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

	struct BaseEvent {
		virtual ~BaseEvent() = default;
		virtual EventType get_type() const = 0;
	};

	struct EventMouseMoved : public BaseEvent {
		EventMouseMoved(const double& new_x, const double& new_y) :
			x(new_x), y(new_y) {}


		virtual EventType get_type() const override { return type; }

		static const EventType type=EventType::MouseMoved;
		double x;
		double y;
	};

	struct EventWindowResize : public BaseEvent {
		EventWindowResize(const unsigned int& new_width, const unsigned int& new_height) :
			width(new_width), height(new_height) {}


		virtual EventType get_type() const override { return type; }

		static const EventType type = EventType::WindowResize;
		unsigned int width;
		unsigned int height;
	};



	struct Event{
		unsigned int width;
		unsigned int height;
	};
}
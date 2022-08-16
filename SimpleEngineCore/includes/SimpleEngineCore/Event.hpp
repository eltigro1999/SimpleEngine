#pragma once
#include <array>
#include <functional>

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

	struct EventWindowClose : public BaseEvent {

		virtual EventType get_type() const override { return type; }

		static const EventType type = EventType::WindowClose;
	};

	class EventDispatcher {
	public:
		template<typename EventType>
		void add_event_listener(std::function<void(EventType&)> callback) {
			auto baseCallback = [func = std::move(callback)](BaseEvent& e) {
				if (e.get_type() == EventType::type) {
					func(static_cast<EventType&>(e));
				}
			};
			m_eventCallbacks[static_cast<size_t>(EventType::type)]=std::move(baseCallback);
		}

		void dispatch(BaseEvent& _event) {
			auto& callback = m_eventCallbacks[static_cast<size_t>(_event.get_type())];
			if (callback) {
				callback(_event);
			}
		}
	private:
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_eventCallbacks;
	};


	struct Event{
		unsigned int width;
		unsigned int height;
	};
}
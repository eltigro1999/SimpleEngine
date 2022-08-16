#include "SimpleEngineCore/Application.hpp"
#include <iostream>
#include "SimpleEngineCore/Log.hpp"
#include "SimpleEngineCore/Window.hpp"

namespace SimpleEngine{
	Application::Application() {
        LOG_INFO("Starting application");
	}
    
	Application::~Application() {
        LOG_INFO("Closing application");

	}

	int Application::start(unsigned int window_width, unsigned int window_height, const char* title) {
		
		m_pWindow=std::make_unique<Window>(title, window_width, window_height);

		m_event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& _event) {
				LOG_INFO("[EVENT] Mouse moved to {0}x{1}", _event.x, _event.y);
			});

		m_event_dispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& _event) {
				LOG_INFO("[EVENT] Changed size {0}x{1}", _event.width, _event.height);
			});

		m_pWindow->set_event_callback(
			[&](BaseEvent& _event) {
				m_event_dispatcher.dispatch(_event);
			});

		while (true) {
			m_pWindow->on_update();
			on_update();
		}
         return 0;
	}
}
#pragma once
#include "SimpleEngineCore/Event.hpp"

#include <string>
#include <functional>
struct GLFWwindow;

namespace SimpleEngine {


	class Window {
		using EventCallbackFn = std::function<void(BaseEvent&)>;
	public:
		Window(const std::string& title, const unsigned int& width, const unsigned int& height);
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;
		~Window();

		void on_update();
		unsigned int get_width() const { return m_data.width; }
		unsigned int get_height() const { return m_data.height; }

		void set_event_callback(const EventCallbackFn& callback)
		{
			m_data.eventCallbackFn = callback;
		}

	private:
		struct WindowData {
			std::string title;
			unsigned int width;
			unsigned int height;
			EventCallbackFn eventCallbackFn;
		};

		WindowData m_data;

		GLFWwindow* m_pWindow=nullptr;
		int init();
		void shutdown();
	};
}
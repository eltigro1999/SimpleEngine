#pragma once
#include <string>

class GLFWwindow;

namespace SimpleEngine {
	class Window {
	public:
		Window(const std::string& title, const unsigned int& width, const unsigned int& height);
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;
		~Window();

		void on_update();
		unsigned int get_width() const { return m_width; }
		unsigned int get_height() const { return m_height; }

	private:
		GLFWwindow* m_pWindow;
		int init();
		void shutdown();
		std::string m_title;
		unsigned int m_width;
		unsigned int m_height;
	};
}
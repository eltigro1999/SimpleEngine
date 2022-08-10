#include "SimpleEngineCore/Window.hpp"
#include "SimpleEngineCore/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace SimpleEngine{

    static bool s_GLFW_initialized=false;

	Window::Window(const std::string& title, const unsigned int& width, const unsigned int& height): 
		m_title(title), m_width(width), m_height(height)
	{
		int returnCode = init();
	}

	int Window::init() {
        LOG_INFO("Creating a window {0} width size {1}x{2}", m_title, m_width, m_height);

        /* Initialize the library */
        if (!s_GLFW_initialized) {
            if (!glfwInit()) {
                LOG_CRITICAL("Can't initialize GLFW!");
                return -1;
            }
            s_GLFW_initialized = true;
        }

        /* Create a windowed mode window and its OpenGL context */
        m_pWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            LOG_CRITICAL("Can't create a window ''{0}'' width size {1}x{2}!", m_title, m_width, m_height);
            glfwTerminate();
            return -2;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Failed to initialize GLAD");
            return -3;
        }

        return 0;
	}

	void Window::on_update() {
        glClearColor(1, 0, 0, 0);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(m_pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

	void Window::shutdown() {
        glfwDestroyWindow(m_pWindow);
        glfwTerminate();
	}

	Window::~Window() {
        shutdown();
    }
}
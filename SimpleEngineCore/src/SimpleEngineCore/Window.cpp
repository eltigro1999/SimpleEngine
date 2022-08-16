#include "SimpleEngineCore/Window.hpp"
#include "SimpleEngineCore/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>

namespace SimpleEngine{

    static bool s_GLFW_initialized=false;

    Window::Window(const std::string& title, const unsigned int& width, const unsigned int& height) :
        m_data({ title, width ,height })
	{
		int returnCode = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
	}

	int Window::init() {
        LOG_INFO("Creating a window {0} width size {1}x{2}", m_data.title, m_data.width, m_data.height);

        /* Initialize the library */
        if (!s_GLFW_initialized) {
            if (!glfwInit()) {
                LOG_CRITICAL("Can't initialize GLFW!");
                return -1;
            }
            s_GLFW_initialized = true;
        }

        /* Create a windowed mode window and its OpenGL context */
        m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_pWindow)
        {
            LOG_CRITICAL("Can't create a window {0} width size {1}x{2}!", m_data.title, m_data.width, m_data.height);
            glfwTerminate();
            return -2;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_pWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("Failed to initialize GLAD");
            return -3;
        }
        glfwSetWindowUserPointer(m_pWindow, &m_data);

        glfwSetWindowSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
                data.width = width;
                data.height = height;

                EventWindowResize _event(width, height);
                data.eventCallbackFn(_event);
            });

            glfwSetCursorPosCallback(m_pWindow, 
                [](GLFWwindow* pWindow, double x, double y)
                {
                    WindowData& data = *(static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow)));
                    
                    EventMouseMoved _event(x,y);
                    data.eventCallbackFn(_event);
                }
        );

            glfwSetWindowCloseCallback(m_pWindow,
                [](GLFWwindow* pWindow)
                {
                    WindowData& data = *(static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow)));

                    EventWindowClose _event;
                    data.eventCallbackFn(_event);
                }
            );

        return 0;
	}

	void Window::on_update() {
        glClearColor(1, 0, 0, 0);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(get_width());
        io.DisplaySize.y = static_cast<float>(get_height());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        
        ImGui::ShowDemoWindow();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



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
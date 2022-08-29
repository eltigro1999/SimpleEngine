#include "SimpleEngineCore/Window.hpp"
#include "SimpleEngineCore/Log.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace SimpleEngine{
    static bool s_GLFW_initialized=false;

    GLfloat points[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    const char* vertex_shader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "out vec3 color;"
        "void main(){"
        "   color = vertex_color;"
        "   gl_Position=vec4(vertex_position, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color=vec4(color, 1.0);"
        "}";

    GLuint shader_program;
    GLuint vao;

    Window::Window(const std::string& title, const unsigned int& width, const unsigned int& height) :
        m_data({ title, width ,height })
	{
		int returnCode = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
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
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertex_shader, nullptr);
        glCompileShader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragment_shader, nullptr);
        glCompileShader(fs);
        
        shader_program = glCreateProgram();
        glAttachShader(shader_program, vs);
        glAttachShader(shader_program, fs);
        glLinkProgram(shader_program);

        //After program linking we can delete shaders

        glDeleteShader(vs);
        glDeleteShader(fs);

        GLuint points_vbo=0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);//make this buffer current
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);//make this buffer current
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);//make this buffer current
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);//make this buffer current
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        return 0;
	}

	void Window::on_update() {
        glClearColor(m_background_color[0], m_background_color[1],
            m_background_color[2], m_background_color[3]);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(get_width());
        io.DisplaySize.y = static_cast<float>(get_height());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::ShowDemoWindow();

        ImGui::Begin("Background Color Window");    //Create a window
        ImGui::ColorEdit4("Background Color", m_background_color);  //Create a widget
        ImGui::End();   //Finish the window

        
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
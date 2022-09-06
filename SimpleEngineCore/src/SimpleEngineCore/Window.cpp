#include "SimpleEngineCore/Window.hpp"
#include "SimpleEngineCore/Log.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexArray.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace SimpleEngine {
    static bool s_GLFW_initialized = false;

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

    GLfloat positions_colors[] = {
        0.0f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
       -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f
    };

    const char* vertex_shader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "out vec3 color;"
        "void main(){"
        "   color = vertex_color;"
        "   gl_Position = vec4(vertex_position, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = vec4(color, 1.0);"
        "}";

    std::unique_ptr<ShaderProgram> p_shader_program;
    std::unique_ptr<VertexBuffer> p_points_vbo;
    std::unique_ptr<VertexBuffer> p_colors_vbo;
    //std::unique_ptr<VertexArray>p_vao;

    std::unique_ptr<VertexArray>p_vao_2buffers;
    std::unique_ptr<VertexArray>p_vao_1buffer;

    std::unique_ptr<VertexBuffer>p_positions_color_vbo;

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

                EventMouseMoved _event(x, y);
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

        glfwSetFramebufferSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height)
            {
                glViewport(0, 0, width, height);
            }
        );


        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->isCompiled()) {
            return false;
        }


        BufferLayout buffer_layout_1vec3{ ShaderDataType::Float3 };

        p_vao_2buffers = std::make_unique<VertexArray>();
        p_points_vbo = std::make_unique<VertexBuffer>(points, sizeof(points), buffer_layout_1vec3);
        p_colors_vbo = std::make_unique<VertexBuffer>(colors, sizeof(colors), buffer_layout_1vec3);
        

        p_vao_2buffers->addBuffer(*p_points_vbo);
        p_vao_2buffers->addBuffer(*p_colors_vbo);

        BufferLayout buffer_layout_2vec3{
            ShaderDataType::Float3,
            ShaderDataType::Float3,
        };

        p_vao_1buffer = std::make_unique<VertexArray>();
        p_positions_color_vbo = std::make_unique<VertexBuffer>(positions_colors, sizeof(positions_colors), buffer_layout_2vec3);

        p_vao_1buffer->addBuffer(*p_positions_color_vbo);


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

        //ImGui::ShowDemoWindow();

        ImGui::Begin("Background Color Window");    //Create a window
        ImGui::ColorEdit4("Background Color", m_background_color);  //Create a widget

        static bool use_2_buffers = true;
        ImGui::Checkbox("2 Buffers", &use_2_buffers);

        if (use_2_buffers) {
            p_shader_program->bind();
            p_vao_2buffers->bind();
            glDrawArrays(GL_TRIANGLES, 0,3);
        }
        else {
            p_shader_program->bind();
            p_vao_1buffer->bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

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
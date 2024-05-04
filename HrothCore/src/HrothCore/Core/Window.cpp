#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Window.hpp"
#include "HrothCore/Core/Application.hpp"

#include <GLFW/glfw3.h>

namespace HrothCore
{
    Window::Window(const WindowProps &props)
        : m_Props(props)
    {
        Init(props);
    }

    Window::~Window()
    {
        Shutdown();
    }

    void Window::Init(const WindowProps &props)
    {
        m_Props.Title = props.Title;
        m_Props.Width = props.Width;
        m_Props.Height = props.Height;
        m_Props.VSync = props.VSync;

        int success = glfwInit();
        if (!success)
        {
            // ASSERT
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwSetErrorCallback([](int error, const char* description)
        {
            // LOG ERROR
        });

        m_Window = glfwCreateWindow(m_Props.Width, m_Props.Height, m_Props.Title.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            glfwTerminate();
            // ASSERT
        }

        EnableVSync(m_Props.VSync);

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
            Application::Get().Close();
		});
    }

    void Window::Shutdown()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Window::EnableVSync(bool enable)
    {
        if (m_Props.VSync == enable)
            return;

        glfwSwapInterval(enable ? 1 : 0);

        m_Props.VSync = enable;
    }

    void Window::Update()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}
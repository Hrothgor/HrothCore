#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Window.hpp"
#include "HrothCore/Core/Application.hpp"

#include <GLFW/glfw3.h>

namespace HrothCore
{
    Window::Window(const WindowProps &props, WindowMode mode)
        : m_Props(props)
    {
        Init(props);
        SetWindowMode(mode);
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
        HC_ASSERT(success == GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwSetErrorCallback([](int error, const char* description)
        {
            // LOG ERROR
        });

        m_Monitor = glfwGetPrimaryMonitor();
        if (!m_Monitor)
        {
            HC_ASSERT(m_Monitor);
            Application::Get().Close();
            return;
        }
        m_VideoMode = glfwGetVideoMode(m_Monitor);
        if (!m_VideoMode)
        {
            HC_ASSERT(m_VideoMode);
            Application::Get().Close();
            return;
        }
        glfwWindowHint(GLFW_RED_BITS, m_VideoMode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, m_VideoMode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, m_VideoMode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, m_VideoMode->refreshRate);

        m_Window = glfwCreateWindow(m_Props.Width, m_Props.Height, m_Props.Title.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            HC_ASSERT(m_Window);
            Application::Get().Close();
            return;
        }

        glfwMakeContextCurrent(m_Window);

        glfwSetWindowUserPointer(m_Window, &m_Props);
        EnableVSync(m_Props.VSync);

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
            Application::Get().Close();
		});

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
		});
    }

    void Window::Shutdown()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Window::EnableVSync(bool enable)
    {
        glfwSwapInterval(enable ? 1 : 0);
        m_Props.VSync = enable;
    }

    void Window::SetWindowMode(WindowMode mode)
    {
        if (m_CurrentWindowMode == mode)
            return;
        switch (mode)
        {
            case WindowMode::Windowed:
            {
                glfwSetWindowMonitor(m_Window, nullptr, m_OldWindowedPos[0], m_OldWindowedPos[1], m_OldWindowedSize[0], m_OldWindowedSize[1], m_VideoMode->refreshRate);
                glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_TRUE);
                break;
            }
            case WindowMode::Fullscreen:
            {
                if (m_CurrentWindowMode == WindowMode::Windowed)
                {
                    glfwGetWindowPos(m_Window, &m_OldWindowedPos[0], &m_OldWindowedPos[1]);
                    glfwGetWindowSize(m_Window, &m_OldWindowedSize[0], &m_OldWindowedSize[1]);
                }
                glfwSetWindowMonitor(m_Window, m_Monitor, 0, 0, m_VideoMode->width, m_VideoMode->height, m_VideoMode->refreshRate);
                glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_TRUE);
                break;
            }
            case WindowMode::Borderless:
            {
                if (m_CurrentWindowMode == WindowMode::Windowed)
                {
                    glfwGetWindowPos(m_Window, &m_OldWindowedPos[0], &m_OldWindowedPos[1]);
                    glfwGetWindowSize(m_Window, &m_OldWindowedSize[0], &m_OldWindowedSize[1]);
                }
                glfwSetWindowMonitor(m_Window, nullptr, 0, 0, m_VideoMode->width, m_VideoMode->height, m_VideoMode->refreshRate);
                glfwSetWindowAttrib(m_Window, GLFW_DECORATED, GLFW_FALSE);
                break;
            }
            default:
                break;
        }
        m_CurrentWindowMode = mode;
    }

    void Window::Update()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
        m_DeltaTime = (float)glfwGetTime() - m_LastFrameTime;
        m_LastFrameTime = (float)glfwGetTime();

        if (glfwGetKey(m_Window, GLFW_KEY_U) == GLFW_PRESS)
        {
            SetWindowMode(WindowMode::Windowed);
        }
        if (glfwGetKey(m_Window, GLFW_KEY_I) == GLFW_PRESS)
        {
            SetWindowMode(WindowMode::Fullscreen);
        }
        if (glfwGetKey(m_Window, GLFW_KEY_O) == GLFW_PRESS)
        {
            SetWindowMode(WindowMode::Borderless);
        }
    }
}
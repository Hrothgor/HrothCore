#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Window.hpp"
#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Events/WindowEvent.hpp"
#include "HrothCore/Events/KeyMouseEvent.hpp"

#include <GLFW/glfw3.h>

namespace HrothCore
{
    Window::Window(const WindowProps &props, WindowMode mode)
        : m_Props(props)
    {
        m_RenderContext = std::make_unique<RenderContext>(this);
        HC_ASSERT(m_RenderContext, "Failed to create render context!");
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
        HC_ASSERT(success == GLFW_TRUE, "Failed to initialize GLFW!");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwSetErrorCallback([](int error, const char* description)
        {
            HC_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
        });

        m_Monitor = glfwGetPrimaryMonitor();
        if (!m_Monitor)
        {
            HC_ASSERT(m_Monitor, "Failed to get primary monitor!");
            Application::Get().Close();
            return;
        }
        m_VideoMode = glfwGetVideoMode(m_Monitor);
        if (!m_VideoMode)
        {
            HC_ASSERT(m_VideoMode, "Failed to get video mode!");
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
            HC_ASSERT(m_Window, "Failed to create window!");
            Application::Get().Close();
            return;
        }

        m_RenderContext->Init();

        glfwSetWindowUserPointer(m_Window, &m_Props);
        EnableVSync(m_Props.VSync);

        SetupCallbacks();
    }

    void Window::Shutdown()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
        m_RenderContext->Shutdown();
    }

    void Window::EnableVSync(bool enable)
    {
        glfwSwapInterval(enable ? 1 : 0);
        m_Props.VSync = enable;
    }

    void Window::HideCursor()
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Window::ShowCursor()
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Window::SetCursorPos(double xpos, double ypos)
    {
        glfwSetCursorPos(m_Window, xpos, ypos);
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
        m_RenderContext->SwapBuffers();
        m_DeltaTime = glfwGetTime() - m_LastFrameTime;
        m_LastFrameTime = glfwGetTime();
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::Close()
    {
        glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
    }

    void Window::SetupCallbacks()
    {
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
            HC_DISPATCH_EVENT(WindowCloseEvent());
		});

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

            HC_DISPATCH_EVENT(WindowResizeEvent(width, height));
		});

        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
        {
            if (focused)
            {
                HC_DISPATCH_EVENT(WindowFocusEvent());
            }
            else
            {
                HC_DISPATCH_EVENT(WindowLostFocusEvent());
            }
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    HC_DISPATCH_EVENT(KeyPressedEvent((KeyCode)key, (ModCode)mods));
                    break;
                }
                case GLFW_RELEASE:
                {
                    HC_DISPATCH_EVENT(KeyReleasedEvent((KeyCode)key, (ModCode)mods));
                    break;
                }
                case GLFW_REPEAT:
                {
                    HC_DISPATCH_EVENT(KeyRepeatedEvent((KeyCode)key, (ModCode)mods));
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    HC_DISPATCH_EVENT(MousePressedEvent((MouseButton)button, (ModCode)mods));
                    break;
                }
                case GLFW_RELEASE:
                {
                    HC_DISPATCH_EVENT(MousePressedEvent((MouseButton)button, (ModCode)mods));
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            HC_DISPATCH_EVENT(MouseScrolledEvent(xOffset, yOffset));
        });
    }
}
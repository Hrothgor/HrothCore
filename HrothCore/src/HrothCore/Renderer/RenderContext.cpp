#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/RenderContext.hpp"
#include "HrothCore/Core/Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace HrothCore
{
    RenderContext::RenderContext(Window *windowHandle)
        : m_WindowHandle(windowHandle)
    {
        HC_ASSERT(m_WindowHandle, "RenderContext: Window handle is null!");
    }

    void RenderContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle->GetNativeWindow());

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            HC_ASSERT(false, "Failed to initialize GLAD");
            return;
        }

        HC_LOG_INFO("OpenGL Info:");
        HC_LOG_INFO("  Vendor: {0}", (const char *)glGetString(GL_VENDOR));
        HC_LOG_INFO("  Renderer: {0}", (const char *)glGetString(GL_RENDERER));
        HC_LOG_INFO("  Version: {0}", (const char *)glGetString(GL_VERSION));

        HC_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "HrothCore requires at least OpenGL version 4.5!");
    }

    void RenderContext::Shutdown()
    {
    }

    void RenderContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle->GetNativeWindow());
    }
}
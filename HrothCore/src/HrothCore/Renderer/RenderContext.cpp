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
        m_Vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        HC_LOG_INFO("  Vendor: {0}", m_Vendor.c_str());
        HC_LOG_INFO("  Renderer: {0}", (const char *)glGetString(GL_RENDERER));
        HC_LOG_INFO("  Version: {0}", (const char *)glGetString(GL_VERSION));

        GLint availableKb = -1;
        if (m_Vendor.find("NVIDIA") != std::string::npos)
            glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &availableKb);
        if (m_Vendor.find("AMD") != std::string::npos)
            glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, &availableKb);
        m_VramAvailableKb = availableKb;

        HC_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "HrothCore requires at least OpenGL version 4.5!");
    }

    void RenderContext::Shutdown()
    {
    }

    void RenderContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle->GetNativeWindow());
    }

    int32_t RenderContext::VramAvailableKb()
    {
        return m_VramAvailableKb;
    }

    int32_t RenderContext::VramUsedKb()
    {
        GLint availableKb = -1;
        if (m_Vendor.find("NVIDIA") != std::string::npos)
            glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &availableKb);
        if (m_Vendor.find("AMD") != std::string::npos)
            glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, &availableKb);
        return std::max(m_VramAvailableKb - availableKb, 0);
    }
}
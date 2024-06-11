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
    
#if DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
        {
            std::string sourceStr;
            switch (source)
            {
                case GL_DEBUG_SOURCE_API: sourceStr = "API"; break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "Window System"; break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Shader Compiler"; break;
                case GL_DEBUG_SOURCE_THIRD_PARTY: sourceStr = "Third Party"; break;
                case GL_DEBUG_SOURCE_APPLICATION: sourceStr = "Application"; break;
                case GL_DEBUG_SOURCE_OTHER: sourceStr = "Other"; break;
                default: sourceStr = "Unknown"; break;
            }
            std::string typeStr;
            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR: typeStr = "Error"; break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated Behavior"; break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = "Undefined Behavior"; break;
                case GL_DEBUG_TYPE_PORTABILITY: typeStr = "Portability"; break;
                case GL_DEBUG_TYPE_PERFORMANCE: typeStr = "Performance"; break;
                case GL_DEBUG_TYPE_MARKER: typeStr = "Marker"; break;
                case GL_DEBUG_TYPE_PUSH_GROUP: typeStr = "Push Group"; break;
                case GL_DEBUG_TYPE_POP_GROUP: typeStr = "Pop Group"; break;
                case GL_DEBUG_TYPE_OTHER: typeStr = "Other"; break;
                default: typeStr = "Unknown"; break;
            }
            switch (severity)
            {
                case GL_DEBUG_SEVERITY_HIGH:
                    HC_LOG_ERROR("OpenGL Debug High: {0} {1} {2}", sourceStr, typeStr, message);
                    break;
                case GL_DEBUG_SEVERITY_MEDIUM:
                    HC_LOG_WARNING("OpenGL Debug Medium: {0} {1} {2}", sourceStr, typeStr, message);
                    break;
                case GL_DEBUG_SEVERITY_LOW:
                    HC_LOG_INFO("OpenGL Debug Low: {0} {1} {2}", sourceStr, typeStr, message);
                    break;
            }
        }, nullptr);
#endif
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
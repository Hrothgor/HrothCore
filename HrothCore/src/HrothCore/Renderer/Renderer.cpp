#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Renderer.hpp"
#include "HrothCore/Renderer/RenderCommand.hpp"
#include "HrothCore/Renderer/VertexArray.hpp"
#include "HrothCore/Renderer/Shader.hpp"

#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Core/Window.hpp"
#include "HrothCore/Events/WindowEvent.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    void Renderer::Init()
    {
        m_FramebufferSize.x = Application::Get().GetWindow().GetWidth();
        m_FramebufferSize.y = Application::Get().GetWindow().GetHeight();
		HC_REGISTER_EVENT(WindowResizeEvent, [&](const WindowResizeEvent& event) -> bool
        {
            m_FramebufferSize.x = event.Width;
            m_FramebufferSize.y = event.Height;
            return true;
        });

        std::vector<Vertex> vertices = {
            { {-0.6, -0.4, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0} },
            { { 0.6, -0.4, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0} },
            { { 0.0,  0.6, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0} }
        };

        m_VAO = std::make_shared<VertexArray>(vertices);

        m_BasicShader = std::make_shared<Shader>("./assets/shaders/Basic.vert", "./assets/shaders/Basic.frag");
        m_BasicShader->Start();
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::RenderScene()
    {
        RenderCommand::SetViewport(m_FramebufferSize);
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        RenderCommand::Clear();

        m_VAO->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}
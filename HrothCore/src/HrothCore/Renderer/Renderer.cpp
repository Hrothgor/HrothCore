#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Renderer.hpp"
#include "HrothCore/Renderer/RenderCommand.hpp"
#include "HrothCore/Renderer/VertexArray.hpp"
#include "HrothCore/Renderer/Buffer.hpp"
#include "HrothCore/Renderer/Shader.hpp"

#include "HrothCore/Types/Transform.hpp"

#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Core/Window.hpp"
#include "HrothCore/Events/WindowEvent.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    void Renderer::Init()
    {
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.0f, -1.0f);

        m_FramebufferSize.x = Application::Get().GetWindow().GetWidth();
        m_FramebufferSize.y = Application::Get().GetWindow().GetHeight();
		HC_REGISTER_EVENT(WindowResizeEvent, [&](const WindowResizeEvent &event) -> bool
        {
            m_FramebufferSize.x = event.Width;
            m_FramebufferSize.y = event.Height;
            return true;
        });

        std::vector<Vertex> vertices = {
            { {-1.0, -1.0, 1.0}, {1.0, 0.0, 0.0}, {0.0, 0.0} },
            { { 1.0, -1.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 0.0} },
            { { 1.0,  1.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0} },
            { {-1.0,  1.0, 1.0}, {1.0, 1.0, 0.0}, {0.0, 0.0} },

            { {-1.0, -1.0, -1.0}, {1.0, 1.0, 0.0}, {0.0, 0.0} },
            { { 1.0, -1.0, -1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0} },
            { { 1.0,  1.0, -1.0}, {0.0, 1.0, 0.0}, {0.0, 0.0} },
            { {-1.0,  1.0, -1.0}, {1.0, 0.0, 0.0}, {0.0, 0.0} }
        };

        std::vector<uint32_t> indices = {
            // front
            0, 1, 2, 2, 3, 0,
            // right
            1, 5, 6, 6, 2, 1,
            // back
            7, 6, 5, 5, 4, 7,
            // left
            4, 0, 3, 3, 7, 4,
            // bottom
            4, 5, 1, 1, 0, 4,
            // top
            3, 2, 6, 6, 7, 3
        };

        m_VAO = std::make_shared<VertexArray>(vertices, indices);
        m_VAO->Bind();

        m_BasicShader = std::make_shared<Shader>("./assets/shaders/Basic.vert", "./assets/shaders/Basic.frag");
        m_BasicShader->Start();

        m_BufferFrameData = std::make_shared<Buffer<PerFrameData>>(1);
        m_BufferFrameData->BindToShader(0, BufferShaderType::Uniform);
    }

    void Renderer::Shutdown()
    {
    }

    static Transform transform(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    void Renderer::RenderScene(double dt)
    {
        RenderCommand::SetViewport(m_FramebufferSize);
        RenderCommand::Clear();

        transform.Rotate(glm::vec3((float)dt));
        const float aspectRatio = m_FramebufferSize.x / (float)m_FramebufferSize.y;
        PerFrameData perFrameData = {
            .mvp = Math::CreateProjMatrix(45.0f, aspectRatio, 0.1f, 1000.0f) * transform.GetTransformMatrix(),
            .isWireframe = false
        };

        m_BufferFrameData->SetData(1, &perFrameData);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, m_VAO->GetVerticesCount(), GL_UNSIGNED_INT, 0);

        perFrameData.isWireframe = true;
        m_BufferFrameData->SetData(1, &perFrameData);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, m_VAO->GetVerticesCount(), GL_UNSIGNED_INT, 0);
    }
}
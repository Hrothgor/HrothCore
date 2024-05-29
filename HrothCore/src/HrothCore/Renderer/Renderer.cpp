#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Renderer.hpp"
#include "HrothCore/Renderer/RenderCommand.hpp"
#include "HrothCore/Renderer/VertexArray.hpp"
#include "HrothCore/Renderer/Buffer.hpp"
#include "HrothCore/Renderer/Shader.hpp"

#include "HrothCore/Types/Transform.hpp"
#include "HrothCore/Types/Camera.hpp"

#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Core/Window.hpp"
#include "HrothCore/Core/AssetManager.hpp"
#include "HrothCore/Events/WindowEvent.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    static Transform transform;

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

        m_VAO = std::make_shared<VertexArray>();
        m_VAO->Bind();

        m_BasicShader = std::make_shared<Shader>("./assets/shaders/Basic.vert", "./assets/shaders/Basic.frag");
        m_BasicShader->Start();

        m_BufferFrameData = std::make_shared<Buffer<PerFrameData>>(1);
        m_BufferFrameData->BindToShader(0, BufferShaderType::Uniform);

        transform.SetScale(glm::vec3(5.0f));
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::RenderScene(std::shared_ptr<Camera> &camera)
    {
        RenderCommand::SetViewport(m_FramebufferSize);
        RenderCommand::Clear();

        const float aspectRatio = m_FramebufferSize.x / (float)m_FramebufferSize.y;

        AssetRef<Model> bunnyref = AssetManager::Get().GetModelRef("./assets/models/bunny/bunny.obj");
        // AssetRef<Model> modelref = AssetManager::Get().GetModelRef("C:/Users/hrothgor/Downloads/game_ready_scifi_helmet/scene.gltf");
        
        transform.Rotate(glm::vec3(0.05f, 0.05f, 0.05f));

        // draw as grid
        for (int i = 0 ; i < 5; i++)
        {
            for (int j = 0 ; j < 5; j++)
            {
                transform.SetPosition(glm::vec3(-25.0 + i * 10.0f, -25.0 + j * 10.0f, -50.0f));
                PerFrameData perFrameData = {
                    .model = transform.GetTransformMatrix(),
                    .view = camera->GetViewMatrix(),
                    .proj = camera->GetProjMatrix(aspectRatio),
                    .isWireframe = false
                };

                m_BufferFrameData->SetData(1, &perFrameData);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glDrawElements(GL_TRIANGLES, bunnyref.Get().GetMesh(0).IndicesCount, GL_UNSIGNED_INT, 0);
            }
        }

        // perFrameData.isWireframe = true;
        // m_BufferFrameData->SetData(1, &perFrameData);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // glDrawElements(GL_TRIANGLES, m_VAO->GetVerticesCount(), GL_UNSIGNED_INT, 0);
    }
}
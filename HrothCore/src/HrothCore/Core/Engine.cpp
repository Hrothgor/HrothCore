#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Engine.hpp"
#include "HrothCore/Core/IClient.hpp"

#include "HrothCore/Renderer/Renderer.hpp"
#include "HrothCore/Renderer/ImGuiLayer.hpp"

namespace HrothCore
{
    void Engine::Init(std::shared_ptr<IClient> &client)
    {
        HC_ASSERT(client != nullptr);

        Renderer::Get().Init();
        ImGuiLayer::Get().Init();

        m_Client = client;
        m_Client->Init();
    }

    void Engine::Shutdown()
    {
        m_Client->Shutdown();
        ImGuiLayer::Get().Shutdown();
        Renderer::Get().Shutdown();
    }

    void Engine::Update(float dt)
    {
        m_Client->Update(dt);
        HC_LOG_INFO("Engine::Update: {0} ms", dt);
        HC_LOG_INFO("Engine::Update: {0} fps", GetFPS());

        if (m_Camera && m_Camera->HasPositioner())
            Renderer::Get().RenderScene(m_Camera);
        else 
            HC_LOG_ERROR("Engine::Update: Camera has no positioner can't render scene!");

        ImGuiLayer::Get().BeginFrame();
        m_Client->ImGuiRender();
        ImGuiLayer::Get().EndFrame();

        m_FPSCounter.Tick(dt);
    }

    void Engine::SetCameraPositioner(ICameraPositioner *cameraPositioner)
    {
        if (m_Camera)
            m_Camera->SetPositioner(cameraPositioner);
        else
            m_Camera = std::make_shared<Camera>(cameraPositioner);
    }
}
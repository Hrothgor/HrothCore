#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Engine.hpp"
#include "HrothCore/Core/IClient.hpp"

#include "HrothCore/Renderer/Renderer.hpp"
#include "HrothCore/Renderer/ImGuiLayer.hpp"

#include "HrothCore/Scene/Scene.hpp"
#include "HrothCore/Scene/GameObject.hpp"

#include "HrothCore/Types/Camera.hpp"

namespace HrothCore
{
    static ICameraPositioner *s_CameraPositioner = nullptr;
    void Engine::Init(std::shared_ptr<IClient> &client)
    {
        HC_ASSERT(client != nullptr, "Engine: Need a client to run!");

        Renderer::Get().Init();
        ImGuiLayer::Get().Init();

        m_Scene = std::make_shared<Scene>();

        GameObject *a = m_Scene->Instantiate("a");
        m_Scene->Instantiate();
        m_Scene->Instantiate();
        m_Scene->Instantiate();
        m_Scene->Instantiate();
        m_Scene->Instantiate();
        m_Scene->Instantiate();

        m_Scene->Instantiate(a);
        m_Scene->Instantiate(a);
        GameObject *b = m_Scene->Instantiate("b", a);
        m_Scene->Instantiate(a);
        m_Scene->Instantiate(a);
        m_Scene->Instantiate(a);

        GameObject *c = m_Scene->Instantiate("c", b);
        m_Scene->Instantiate(b);
        m_Scene->Instantiate(b);

        m_Scene->Instantiate(a);
        m_Scene->Instantiate(b);
        m_Scene->Instantiate(c);
        m_Scene->Instantiate("r1");
        m_Scene->Instantiate("r2");
        m_Scene->Instantiate("r3");

        s_CameraPositioner = new CameraPositionerEditor();
        SetCameraPositioner(s_CameraPositioner);

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
        ImGuiLayer::Get().Render();
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
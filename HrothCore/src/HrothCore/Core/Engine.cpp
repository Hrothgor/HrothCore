#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Engine.hpp"
#include "HrothCore/Core/IClient.hpp"
#include "HrothCore/Core/Scene.hpp"

#include "HrothCore/Types/Camera.hpp"

#include "HrothCore/Renderer/Renderer.hpp"
#include "HrothCore/Renderer/ImGuiLayer.hpp"

namespace HrothCore
{
    void Engine::Init(std::shared_ptr<IClient> &client)
    {
        HC_ASSERT(client != nullptr);

        Renderer::Get().Init();
        ImGuiLayer::Get().Init();

        m_Scene = std::make_shared<Scene>();

        m_Client = client;
        m_Client->Init();

        // TEST SCENE
        int a = m_Scene->AddNode(0);
        m_Scene->AddNode(0);
        m_Scene->AddNode(0);
        m_Scene->AddNode(0);
        m_Scene->AddNode(0);
        m_Scene->AddNode(0);

        m_Scene->SetNodeName(a, "A");

        m_Scene->AddNode(a);
        int b = m_Scene->AddNode(a);
        m_Scene->AddNode(a);
        m_Scene->AddNode(a);
        m_Scene->AddNode(a);

        m_Scene->SetNodeName(b, "B");

        m_Scene->AddNode(b);
        m_Scene->AddNode(b);
        m_Scene->AddNode(b);
        m_Scene->AddNode(b);
        int c = m_Scene->AddNode(b);

        m_Scene->SetNodeName(c, "C");

        m_Scene->AddNode(c);
        int d = m_Scene->AddNode(c);
        m_Scene->AddNode(c);

        m_Scene->SetNodeName(d, "D");

        m_Scene->AddNode(d);

        m_Scene->AddNode(a);
        m_Scene->AddNode(a);
        m_Scene->AddNode(0);
        m_Scene->AddNode(c);

        m_Scene->DumpToDot("./scene.dot");
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
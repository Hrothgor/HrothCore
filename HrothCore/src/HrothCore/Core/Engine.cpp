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
        NodeView a = m_Scene->AddNode();
        m_Scene->AddNode();
        m_Scene->AddNode();
        m_Scene->AddNode();
        m_Scene->AddNode();
        m_Scene->AddNode();

        m_Scene->SetNodeName(a, "A");

        m_Scene->AddNode(a);
        NodeView b = m_Scene->AddNode(a);
        m_Scene->AddNode(a);
        m_Scene->AddNode(a);
        m_Scene->AddNode(a);

        m_Scene->SetNodeName(b, "B");

        m_Scene->AddNode(b);
        m_Scene->AddNode(b);
        m_Scene->AddNode(b);
        m_Scene->AddNode(b);
        NodeView c = m_Scene->AddNode(b);

        m_Scene->SetNodeName(c, "C");

        m_Scene->AddNode(c);
        NodeView d = m_Scene->AddNode(c);
        m_Scene->AddNode(c);

        m_Scene->SetNodeName(d, "D");

        m_Scene->AddNode(d);

        m_Scene->AddNode(a);
        m_Scene->AddNode(a);
        m_Scene->AddNode();
        m_Scene->AddNode(c);

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 10000000; i++)
        {
            m_Scene->RemoveNode(c);
        }
        auto elapsed = std::chrono::high_resolution_clock::now() - start;

        m_Scene->DumpToDot("./scene.dot");
        long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();

        HC_LOG_WARNING("Engine::Init: {0} microseconds", microseconds);

        abort();
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
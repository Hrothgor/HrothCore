#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Engine.hpp"
#include "HrothCore/Core/IClient.hpp"

#include "HrothCore/Renderer/Renderer.hpp"
#include "HrothCore/Renderer/ImGuiLayer.hpp"

#include "HrothCore/Scene/Scene.hpp"
#include "HrothCore/Scene/GameObject.hpp"

#include "HrothCore/Types/Camera.hpp"

// TEMP
#include "HrothCore/Scene/GameObject.hpp"
#include "HrothCore/Components/TransformComponent.hpp"
#include "HrothCore/Components/StaticMeshComponent.hpp"
#include "HrothCore/Core/AssetManager.hpp"
//

namespace HrothCore
{
    void Engine::Init(std::shared_ptr<IClient> &client)
    {
        HC_ASSERT(client != nullptr, "Engine: Need a client to run!");

        Renderer::Init();
        ImGuiLayer::Init();

        m_Scene = new Scene();

        m_Client = client;
        m_Client->Init();
    }

    void Engine::Shutdown()
    {
        m_Client->Shutdown();
        ImGuiLayer::Shutdown();
        Renderer::Shutdown();

        delete m_Scene;
    }

    void Engine::Update(float dt)
    {
        if (m_Camera && m_Scene)
            m_Scene->Render(*m_Camera);
        else 
            HC_LOG_ERROR("Engine::Update: No Camera to render the scene!");

        ImGuiLayer::BeginFrame();
        ImGuiLayer::Render(dt);
        ImGuiLayer::EndFrame();

        m_FPSCounter.Tick(dt);

        m_Client->Update(dt);
    }

    void Engine::SetCameraPtr(Camera *camera)
    {
        m_Camera = camera;
    }
}
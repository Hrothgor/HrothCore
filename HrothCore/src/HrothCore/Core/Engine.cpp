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
        ImGuiLayer::Get().Init();

        m_Scene = std::make_shared<Scene>();

        // TEMP
        AssetRef<Model> bunnyRef = AssetManager::Get().GetModelRef("./assets/models/bunny/bunny.obj");
        AssetRef<Model> dragonRef = AssetManager::Get().GetModelRef("./assets/models/dragon/dragon.obj");
        for (int x = 0; x < 4; x++)
            for (int y = 0; y < 4; y++)
                for (int z = 0; z < 4; z++) {
                    GameObject *go = m_Scene->Instantiate();
                    go->GetComponent<TransformComponent>().SetPosition(glm::vec3(x * 3.5f, y * 3.5f, z * 3.5f));
                    if ((x + y + z) % 2 == 0)
                    {
                        go->GetComponent<TransformComponent>().SetScale(glm::vec3(3.0f, 3.0f, 3.0f));
                        go->AddComponent<StaticMeshComponent>(dragonRef);
                    }
                    else
                        go->AddComponent<StaticMeshComponent>(bunnyRef);
                }
        //

        m_Client = client;
        m_Client->Init();
    }

    void Engine::Shutdown()
    {
        m_Client->Shutdown();
        ImGuiLayer::Get().Shutdown();
        Renderer::Shutdown();
    }

    void Engine::Update(float dt)
    {
        if (m_Camera)
            m_Scene->Render(*m_Camera);
        else 
            HC_LOG_ERROR("Engine::Update: No Camera to render the scene!");

        ImGuiLayer::Get().BeginFrame();
        ImGuiLayer::Get().Render(dt);
        ImGuiLayer::Get().EndFrame();

        m_FPSCounter.Tick(dt);

        m_Client->Update(dt);
    }

    void Engine::SetCameraPtr(Camera *camera)
    {
        m_Camera = camera;
    }
}
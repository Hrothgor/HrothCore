#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Engine.hpp"
#include "HrothCore/Core/IClient.hpp"

#include "HrothCore/Renderer/Renderer.hpp"

namespace HrothCore
{
    void Engine::Init(std::shared_ptr<IClient> &client)
    {
        HC_ASSERT(client != nullptr);

        Renderer::Get().Init();

        m_Client = client;
        m_Client->Init();
    }

    void Engine::Shutdown()
    {
        m_Client->Shutdown();
        Renderer::Get().Shutdown();
    }

    void Engine::Update(double dt)
    {
        m_Client->Update(dt);
        HC_LOG_DEBUG("Engine::Update: {0} ms", dt);
        HC_LOG_DEBUG("Engine::Update: {0} fps", 1.0/dt);

        Renderer::Get().RenderScene(dt);
    }
}
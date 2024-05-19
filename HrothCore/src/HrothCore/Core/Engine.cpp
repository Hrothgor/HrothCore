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

    static int averagefps =0;
    static int frames = 0;

    void Engine::Shutdown()
    {
        HC_LOG_WARNING("Average FPS: {0}", averagefps/frames);
        m_Client->Shutdown();
        Renderer::Get().Shutdown();
    }


    void Engine::Update(double dt)
    {
        m_Client->Update(dt);
        HC_LOG_INFO("Engine::Update: {0} ms", dt);
        HC_LOG_INFO("Engine::Update: {0} fps", 1.0/dt);

        averagefps += 1.0/dt;
        frames++;

        Renderer::Get().RenderScene(dt);
    }
}
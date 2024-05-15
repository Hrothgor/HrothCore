#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Engine.hpp"

namespace HrothCore
{
    void Engine::Init()
    {

    }

    void Engine::Shutdown()
    {

    }

    void Engine::Update(double dt)
    {
        HC_LOG_DEBUG("Engine::Update: {0} ms", dt);
        HC_LOG_DEBUG("Engine::Update: {0} fps", 1.0/dt);
    }
}
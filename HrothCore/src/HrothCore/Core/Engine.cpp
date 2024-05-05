#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Engine.hpp"

namespace HrothCore
{
    Engine::Engine()
    {
    }

    void Engine::Init()
    {

    }

    void Engine::Shutdown()
    {

    }

    void Engine::Update(float dt)
    {
        std::cout << dt << std::endl;
        std::cout << 1.0/dt << std::endl;
    }
}
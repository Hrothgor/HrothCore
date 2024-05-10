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
        std::cout << dt << std::endl;
        std::cout << 1.0/dt << std::endl;
    }
}
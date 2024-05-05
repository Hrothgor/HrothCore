#pragma once

#include "HrothCore/Core/Macro.hpp"

namespace HrothCore
{
    class Engine
    {
        HC_SINGLETON(Engine)

        public:
            virtual ~Engine() = default;


            void Init();
            void Shutdown();
            void Update(float dt);

        private:
    };
}
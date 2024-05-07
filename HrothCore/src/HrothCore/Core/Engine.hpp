#pragma once

namespace HrothCore
{
    class Engine
    {
        HC_SINGLETON(Engine)

        public:
            virtual ~Engine() = default;


            void Init();
            void Shutdown();
            void Update(double dt);

        private:
    };
}
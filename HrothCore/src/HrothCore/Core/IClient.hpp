#pragma once

namespace HrothCore
{
    class ImGuiLayer;

    class IClient
    {
        public:
            virtual ~IClient() = default;

            virtual void Init() = 0;
            virtual void Shutdown() = 0;
            virtual void Update(float dt) = 0;
    };
}
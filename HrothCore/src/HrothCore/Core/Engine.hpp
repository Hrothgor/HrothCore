#pragma once

#include "HrothCore/Core/FPSCounter.hpp"

namespace HrothCore
{
    class IClient;
    class ICameraPositioner;
    class Camera;
    class Scene;

    class Engine
    {
        HC_SINGLETON(Engine)

        public:
            virtual ~Engine() = default;

            void Init(std::shared_ptr<IClient> &client);
            void Shutdown();
            void Update(float dt);

            float GetFPS() const { return m_FPSCounter.GetFPS(); }

            void SetCameraPositioner(ICameraPositioner *cameraPositioner);
        private:
            FPSCounter m_FPSCounter;
            std::shared_ptr<IClient> m_Client;

            std::shared_ptr<Camera> m_Camera;

            std::shared_ptr<Scene> m_Scene;
    };
}
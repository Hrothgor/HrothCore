#pragma once

#include "HrothCore/Core/FPSCounter.hpp"

namespace HrothCore
{
    class IClient;
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

            void SetCameraPtr(Camera *camera);

            Scene *GetScene() const { return m_Scene; }
            Scene **GetScenePtr() { return &m_Scene; }

        private:
            FPSCounter m_FPSCounter;
            std::shared_ptr<IClient> m_Client;
            Scene *m_Scene;

            Camera *m_Camera;
    };
}
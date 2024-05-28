#pragma once

namespace HrothCore
{
    class FPSCounter
    {
        public:
            FPSCounter(float FPSUpdateInterval = 0.5f);
            ~FPSCounter() = default;

            void Tick(float dt);
            
            float GetFPS() const { return m_FPS; };

        private:
            const float m_FPSUpdateInterval;
            float m_FPS = 0;
            float m_AccumulatedTime = 0;
            uint32_t m_FrameCount = 0;
    };
}
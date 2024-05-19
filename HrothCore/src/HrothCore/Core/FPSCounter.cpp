#include "HrothCore_pch.hpp"

#include "HrothCore/Core/FPSCounter.hpp"

namespace HrothCore
{
    FPSCounter::FPSCounter(float FPSUpdateInterval)
        : m_FPSUpdateInterval(FPSUpdateInterval)
    {
        HC_ASSERT(FPSUpdateInterval > 0);
    }

    void FPSCounter::Tick(double dt)
    {
        m_FrameCount++;
        m_AccumulatedTime += dt;

        if (m_AccumulatedTime >= m_FPSUpdateInterval)
        {
            m_FPS = ((m_FrameCount / m_AccumulatedTime) + m_FPS) * 0.5f;
            m_AccumulatedTime = 0;
            m_FrameCount = 0;
        }
    }
}
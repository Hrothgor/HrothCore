#pragma once

#include "HrothCore/Renderer/ImGuiLayer.hpp"

namespace HrothCore
{
    class InfoPanel : public HrothCore::ImGuiPanel
    {
        public:
            void OnUpdate() override;
    };
}
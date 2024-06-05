#pragma once

#include "HrothCore/Renderer/ImGuiLayer.hpp"

namespace HrothCore
{
    class InfoPanel : public ImGuiPanel
    {
        public:
            void OnUpdate(float dt) override;
    };
}
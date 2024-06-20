#pragma once

#include "HrothCore/Renderer/ImGuiLayer.hpp"

namespace HrothCore
{
    class AssetBrowserPanel : public ImGuiPanel
    {
        public:
            void OnUpdate(float dt) override;
    };
}
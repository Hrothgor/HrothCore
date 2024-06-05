#pragma once

#include "HrothCore/Renderer/ImGuiLayer.hpp"

namespace HrothCore
{
    class CameraPositionerEditor;

    class ViewportPanel : public ImGuiPanel
    {
        public:
            ViewportPanel(CameraPositionerEditor *cameraPositioner = nullptr);

            void OnUpdate(float dt) override;
        private:
            CameraPositionerEditor *m_CameraPositioner;
    };
}
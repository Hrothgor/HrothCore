#pragma once

#include "HrothCore/Renderer/ImGuiLayer.hpp"

namespace HrothCore
{
    class Scene;
    class Camera;
    class CameraPositionerEditor;

    class ViewportPanel : public ImGuiPanel
    {
        public:
            ViewportPanel(Scene **scenePtr, Camera *camera, CameraPositionerEditor *cameraPositioner = nullptr);

            void OnUpdate(float dt) override;
        private:
            void DrawGuizmo();

            int m_GizmoType;
            bool m_GizmoSnap;

            Scene **m_ScenePtr;
            Camera *m_Camera;
            CameraPositionerEditor *m_CameraPositioner;
    };
}
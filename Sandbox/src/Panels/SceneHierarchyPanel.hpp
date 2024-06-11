#pragma once

#include "HrothCore/Renderer/ImGuiLayer.hpp"

namespace HrothCore
{
    class GameObject;
    class Scene;

    class SceneHierarchyPanel : public ImGuiPanel
    {
        public:
            SceneHierarchyPanel(Scene **scenePtr);

            void OnUpdate(float dt) override;

        private:
            //returns the node's rectangle for tree lines drawing
            ImVec4 TraverseScene(GameObject *parent);

            Scene **m_ScenePtr;
    };
}
#pragma once

#include "HrothCore/Renderer/ImGuiLayer.hpp"

namespace HrothCore
{
    class GameObject;
    class Scene;

    class SceneHierarchyPanel : public ImGuiPanel
    {
        public:
            SceneHierarchyPanel(std::shared_ptr<Scene> scene);

            void OnUpdate(float dt) override;

        private:
            //returns the node's rectangle for tree lines drawing
            ImVec4 TraverseScene(GameObject *parent);

            std::shared_ptr<Scene> m_Scene;
    };
}
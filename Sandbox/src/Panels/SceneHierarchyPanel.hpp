#pragma once

#include "HrothCore/Renderer/ImGuiLayer.hpp"

namespace HrothCore
{
    class GameObject;
    class Scene;
}

class SceneHierarchyPanel : public HrothCore::ImGuiPanel
{
    public:
        SceneHierarchyPanel(std::shared_ptr<HrothCore::Scene> scene);

        void OnUpdate() override;

    private:
        void TraverseScene(HrothCore::GameObject *parent);

        std::shared_ptr<HrothCore::Scene> m_Scene;
};
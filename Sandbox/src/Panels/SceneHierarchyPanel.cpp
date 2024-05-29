#include "Panels/SceneHierarchyPanel.hpp"

#include "HrothCore/Scene/GameObject.hpp"
#include "HrothCore/Components/IDComponent.hpp"

using namespace HrothCore;

SceneHierarchyPanel::SceneHierarchyPanel(std::shared_ptr<Scene> scene)
    : m_Scene(scene)
{
}

void SceneHierarchyPanel::OnUpdate()
{
    ImGui::Begin("Scene Hierarchy");

    TraverseScene(nullptr);
    
    ImGui::BeginChild("Blank Space");
    if (ImGui::BeginPopupContextWindow("_SCENE_HIERARCHY_CONTEXTMENU"))
    {
        if (ImGui::MenuItem("Create Empty Entity"))
            m_Scene->Instantiate();
        ImGui::EndPopup();
    }
    ImGui::EndChild();

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_SCENE_HIERARCHY_ENTITY_DRAG"))
        {
            GameObject *cpy = static_cast<GameObject *>(payload->Data);
            GameObject *ent = m_Scene->Find(cpy->GetComponent<IDComponent>().UUID);
            ent->DetachFromParent();
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}

void SceneHierarchyPanel::TraverseScene(GameObject *parent)
{
    if (!parent)
    {
        for (auto &child : m_Scene->GetRoot()->GetChilds())
            TraverseScene(child);
        return;
    }

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (parent->GetChilds().empty())
        flags |= ImGuiTreeNodeFlags_Leaf;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });
    bool expanded = ImGui::TreeNodeEx((void *)parent, flags, "%s", parent->GetComponent<IDComponent>().Name.c_str());
    ImGui::PopStyleVar();

    if (ImGui::BeginPopupContextItem((char *)parent))
    {
        if (ImGui::MenuItem("Create Empty Entity"))
            m_Scene->Instantiate(parent);
        ImGui::EndPopup();
    }

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("_SCENE_HIERARCHY_ENTITY_DRAG", parent, sizeof(GameObject));
        ImGui::TextUnformatted(parent->GetComponent<IDComponent>().Name.c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_SCENE_HIERARCHY_ENTITY_DRAG"))
        {
            GameObject *cpy = static_cast<GameObject *>(payload->Data);
            GameObject *ent = m_Scene->Find(cpy->GetComponent<IDComponent>().UUID);
            ent->AttachToParent(parent);
        }
        ImGui::EndDragDropTarget();
    }

    if (expanded)
    {
        for (auto &child : parent->GetChilds())
            TraverseScene(child);

        ImGui::TreePop();
    }
}
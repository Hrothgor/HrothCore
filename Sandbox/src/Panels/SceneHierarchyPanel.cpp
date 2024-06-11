#include "HrothCore_pch.hpp"

#include "Panels/SceneHierarchyPanel.hpp"

#include "HrothCore/Scene/GameObject.hpp"
#include "HrothCore/Components/IDComponent.hpp"

namespace HrothCore
{
    SceneHierarchyPanel::SceneHierarchyPanel(Scene **scenePtr)
        : m_ScenePtr(scenePtr)
    {
    }

    void DrawBackGround(int row_count, ImVec4 col_even, ImVec4 col_odd)
    {
        float x1 = ImGui::GetCurrentWindow()->WorkRect.Min.x;
        float x2 = ImGui::GetCurrentWindow()->WorkRect.Max.x;

        float item_spacing_y = ImGui::GetStyle().ItemSpacing.y;
        float item_offset_y = -item_spacing_y * 0.5f;
        float line_height = ImGui::GetTextLineHeight() + item_spacing_y;

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        float y0 = ImGui::GetCursorScreenPos().y + (float)(int)item_offset_y;

        const auto pos = ImGui::GetCursorPos();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImGuiListClipper clipper;
        clipper.Begin(row_count, line_height);
        while (clipper.Step())
        {
            for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; ++row_n)
            {
                ImVec4 col = (row_n & 1) ? col_odd : col_even;
                float y1 = y0 + (line_height * static_cast<float>(row_n));
                float y2 = y1 + line_height;
                draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::GetColorU32(col));
            }
        }
        ImGui::SetCursorPos(pos);
    }

    void SceneHierarchyPanel::OnUpdate(float dt)
    {
        ImGui::Begin("Scene Hierarchy");

        if (!m_ScenePtr || !*m_ScenePtr)
        {
            ImGui::Text("No Scene Selected");
            ImGui::End();
            return;
        }

        DrawBackGround(50, ImGui::GetStyle().Colors[ImGuiCol_WindowBg], ImVec4(0.125f, 0.125f, 0.125f, 1.0f));
        TraverseScene(nullptr);
        
        ImGui::BeginChild("Blank Space");
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
            (*m_ScenePtr)->SetSelectedEntity(nullptr);
        if (ImGui::BeginPopupContextWindow("_SCENE_HIERARCHY_CONTEXTMENU"))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
                (*m_ScenePtr)->Instantiate();
            ImGui::EndPopup();
        }
        ImGui::EndChild();

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_SCENE_HIERARCHY_ENTITY_DRAG"))
            {
                GameObject *cpy = static_cast<GameObject *>(payload->Data);
                GameObject *ent = (*m_ScenePtr)->Find(cpy->GetComponent<IDComponent>().Uuid);
                ent->DetachFromParent();
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::End();
    }

    //returns the node's rectangle for tree lines drawing
    ImVec4 SceneHierarchyPanel::TraverseScene(GameObject *object)
    {
        if (!object)
        {
            for (auto &child : (*m_ScenePtr)->GetRoot()->GetChilds())
                TraverseScene(child);
            return ImVec4();
        }

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
        if (object->GetChilds().empty())
            flags |= ImGuiTreeNodeFlags_Leaf;
        if ((*m_ScenePtr)->GetSelectedEntity() == object)
            flags |= ImGuiTreeNodeFlags_Selected;

        bool expanded = ImGui::TreeNodeEx((void *)object, flags, "%s", object->GetComponent<IDComponent>().Name.c_str());

        if (ImGui::BeginPopupContextItem((char *)object))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
                (*m_ScenePtr)->Instantiate(object);
            ImGui::EndPopup();
        }

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("_SCENE_HIERARCHY_ENTITY_DRAG", object, sizeof(GameObject));
            ImGui::TextUnformatted(object->GetComponent<IDComponent>().Name.c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_SCENE_HIERARCHY_ENTITY_DRAG"))
            {
                GameObject *cpy = static_cast<GameObject *>(payload->Data);
                GameObject *ent = (*m_ScenePtr)->Find(cpy->GetComponent<IDComponent>().Uuid);
                ent->AttachToParent(object);
            }
            ImGui::EndDragDropTarget();
        }

        if (ImGui::IsItemClicked())
            (*m_ScenePtr)->SetSelectedEntity(object);

        const ImVec4 nodeRect = ImVec4(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y, ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y);

        if (expanded)
        {
            const ImColor TreeLineColor = ImColor(200, 200, 200, 255);
            const float SmallOffsetX = 11.0f; //for now, a hardcoded value; should take into account tree indent size ?
            const float SmallOffsetY = 6.0f; //for now, a hardcoded value; should take into account tree indent size ?
            ImDrawList* drawList = ImGui::GetWindowDrawList();

            ImVec2 verticalLineStart = ImGui::GetCursorScreenPos();
            verticalLineStart.x -= SmallOffsetX; //to nicely line up with the arrow symbol
            verticalLineStart.y -= SmallOffsetY; //to nicely line up with the arrow symbol
            ImVec2 verticalLineEnd = verticalLineStart;

            for (auto &child : object->GetChilds())
            {
                const float HorizontalTreeLineSize = 15.0f; //chosen arbitrarily
                const ImVec4 childRect = TraverseScene(child);
                const float midpoint = (childRect.y + childRect.w) / 2.0f; // Min.y and Max.y are the top and bottom of the rectangle
                drawList->AddLine(ImVec2(verticalLineStart.x, midpoint), ImVec2(verticalLineStart.x + HorizontalTreeLineSize, midpoint), TreeLineColor);
                verticalLineEnd.y = midpoint;
            }

            drawList->AddLine(verticalLineStart, verticalLineEnd, TreeLineColor);

            ImGui::TreePop();
        }

        return nodeRect;
    }
}
#include "HrothCore_pch.hpp"

#include "Panels/ViewportPanel.hpp"

#include "HrothCore/Scene/Scene.hpp"

#include "HrothCore/Renderer/Renderer.hpp"
#include "HrothCore/Renderer/Framebuffer.hpp"

#include "HrothCore/Components/TransformComponent.hpp"
#include "HrothCore/Scene/GameObject.hpp"

#include <ImGuizmo.h>

namespace HrothCore
{
    ViewportPanel::ViewportPanel(Scene **scenePtr, Camera *camera, CameraPositionerEditor *cameraPositioner)
        : m_ScenePtr(scenePtr), m_Camera(camera), m_CameraPositioner(cameraPositioner)
    {
    }

    void ViewportPanel::OnUpdate(float dt)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar);

        if (!m_ScenePtr || !*m_ScenePtr)
        {
            ImGui::End();
            ImGui::PopStyleVar();
            return;
        }

        bool isFocus = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();
        if (isFocus && m_CameraPositioner)
            m_CameraPositioner->Update(dt);

        ImVec2 size = ImGui::GetContentRegionAvail();
        ImGui::Image((ImTextureID)(size_t)Renderer::GetFramebuffer(Framebuffers::ScreenView)->GetTexture("Color")->GetID(),
                    size, ImVec2 {0, 1}, ImVec2 {1, 0});
        DrawGuizmo();
        
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void ViewportPanel::DrawGuizmo()
    {
        GameObject *entity = nullptr;

		if (!(entity = (*m_ScenePtr)->GetSelectedEntity()) || m_GizmoType == -1)
            return;

        ImGuizmo::SetDrawlist();
        ImGuizmo::AllowAxisFlip(false);
        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

        glm::mat4 cameraProjection = m_Camera->GetProjMatrix(windowWidth / windowHeight);
        glm::mat4 cameraView = m_Camera->GetViewMatrix();

        TransformComponent &transform = entity->GetComponent<TransformComponent>();
        glm::mat4 transformMat = transform.Global;

        // Snapping
        float snapValue = 0.5;
        if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45;
        
        float snapValues[3] = {snapValue, snapValue, snapValue};

        glm::mat4 localBounds = glm::mat4(1.0f);

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), 
            (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::WORLD, glm::value_ptr(transformMat),
            glm::value_ptr(localBounds), m_GizmoSnap ? snapValues : nullptr);
        
        if (ImGuizmo::IsUsing()) {
            float translation[3];
            float rotation[3];
            float scale[3];

            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transformMat), translation, rotation, scale);

            transform.SetPosition(glm::vec3(translation[0], translation[1], translation[2]));
            transform.SetRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));
            transform.SetScale(glm::vec3(scale[0], scale[1], scale[2]));
        }
    }
}
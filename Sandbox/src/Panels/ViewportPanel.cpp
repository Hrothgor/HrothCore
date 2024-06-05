#include "HrothCore_pch.hpp"

#include "Panels/ViewportPanel.hpp"

#include "HrothCore/Renderer/Renderer.hpp"
#include "HrothCore/Renderer/Framebuffer.hpp"

#include "HrothCore/Types/Camera.hpp"

namespace HrothCore
{
    ViewportPanel::ViewportPanel(CameraPositionerEditor *cameraPositioner)
        : m_CameraPositioner(cameraPositioner)
    {
    }

    void ViewportPanel::OnUpdate(float dt)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar);

        bool isFocus = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();
        if (isFocus && m_CameraPositioner)
            m_CameraPositioner->Update(dt);

        ImVec2 size = ImGui::GetContentRegionAvail();
        ImGui::Image((ImTextureID)(size_t)Renderer::GetFramebuffer(Framebuffers::ScreenView)->GetTexture("Color")->GetID(),
                    size, ImVec2 {0, 1}, ImVec2 {1, 0});
        // DrawGuizmo();
        
        ImGui::End();
        ImGui::PopStyleVar();
    }
}
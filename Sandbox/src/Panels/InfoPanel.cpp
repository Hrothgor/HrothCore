#include "HrothCore_pch.hpp"

#include "Panels/InfoPanel.hpp"

#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Core/Engine.hpp"
#include "HrothCore/Core/Window.hpp"

#include "HrothCore/Renderer/Renderer.hpp"

namespace HrothCore
{
    void InfoPanel::OnUpdate(float dt)
    {
        ImGui::Begin("Info");
        auto &app = Application::Get();
        auto &window = app.GetWindow();

        ImGui::Text("FPS: %.1f", Engine::Get().GetFPS());
        ImGui::Text("Delta: %.3f", dt);

        ImGui::Separator();

        int32_t availableVram = window.GetVramAvailableKb();
        int32_t usedVram = window.GetVramUsedKb();
        ImGui::ProgressBar(usedVram / (float)availableVram);
        ImGui::Text("VRAM: %d/%d KB", usedVram, availableVram);

        ImGui::Separator();

        if (ImGui::Button("Hot reload shaders"))
        {
            Renderer::ReloadAllShader();
        }
    
        ImGui::End();
    }
}
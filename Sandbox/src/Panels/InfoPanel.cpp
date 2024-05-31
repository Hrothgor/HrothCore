#include "HrothCore_pch.hpp"

#include "Panels/InfoPanel.hpp"

#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Core/Window.hpp"

namespace HrothCore
{
    void InfoPanel::OnUpdate()
    {
        ImGui::Begin("Info");
        auto &app = Application::Get();
        auto &window = app.GetWindow();

        ImGui::Text("FPS: %.1f", 1.0f / window.GetDeltaTime());
        ImGui::Text("Delta: %.3f", window.GetDeltaTime());

        ImGui::Separator();

        int32_t availableVram = window.GetVramAvailableKb();
        int32_t usedVram = window.GetVramUsedKb();
        ImGui::ProgressBar(usedVram / (float)availableVram);
        ImGui::Text("VRAM: %d/%d KB", usedVram, availableVram);
    
        ImGui::End();
    }
}
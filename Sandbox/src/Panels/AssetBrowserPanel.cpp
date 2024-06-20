#include "HrothCore_pch.hpp"

#include "Panels/AssetBrowserPanel.hpp"

#include "HrothCore/Core/Application.hpp"
#include "HrothCore/Core/Engine.hpp"
#include "HrothCore/Core/Window.hpp"

#include "HrothCore/Renderer/Renderer.hpp"

namespace HrothCore
{
    void AssetBrowserPanel::OnUpdate(float dt)
    {
        ImGui::Begin("Asset Browser");

        ImGui::Columns(2);
    
        ImGui::End();
    }
}
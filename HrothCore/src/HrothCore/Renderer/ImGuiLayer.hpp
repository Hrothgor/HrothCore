#pragma once

namespace HrothCore
{
    class ImGuiLayer
    {
        HC_SINGLETON(ImGuiLayer)

        public:
            ~ImGuiLayer() = default;

            void Init();
            void Shutdown();
            void BeginFrame();
            void EndFrame();

        private:
            void SetupStyle();
            void SetupDocking();
    };
}
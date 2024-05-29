#pragma once

namespace HrothCore
{
    class ImGuiPanel
    {
        public:
            virtual void OnAttach() {}
            virtual void OnDetach() {}
            virtual void OnUpdate() {}
    };

    class ImGuiLayer
    {
        HC_SINGLETON(ImGuiLayer)

        public:
            ~ImGuiLayer() = default;

            void Init();
            void Shutdown();
            void BeginFrame();
            void Render();
            void EndFrame();

            template<typename T, typename ... Args>
            void RegisterPanel(Args&&... args)
            {
                m_Panels.push_back(std::make_unique<T>(std::forward<Args>(args)...));
                m_Panels.back()->OnAttach();
            }

        private:
            void SetupStyle();
            void SetupDocking();

            std::vector<std::unique_ptr<ImGuiPanel>> m_Panels;
    };
}
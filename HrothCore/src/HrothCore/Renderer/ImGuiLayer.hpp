#pragma once

namespace HrothCore
{
    class ImGuiPanel
    {
        public:
            virtual void OnAttach() {}
            virtual void OnDetach() {}
            virtual void OnUpdate(float dt) {}
    };

    namespace ImGuiLayer
    {
        void Init();
        void Shutdown();

        void BeginFrame();
        void Render(float dt);
        void EndFrame();

        void UseDocking(bool use = true);

        std::vector<std::unique_ptr<ImGuiPanel>> &Panels();

        template<typename T, typename ... Args>
        void RegisterPanel(Args&&... args)
        {
            Panels().push_back(std::make_unique<T>(std::forward<Args>(args)...));
            Panels().back()->OnAttach();
        }
    };
}
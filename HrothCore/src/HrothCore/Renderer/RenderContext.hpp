#pragma once

namespace HrothCore
{
    class Window;

    class RenderContext
    {
        public:
            RenderContext(Window *windowHandle);
            virtual ~RenderContext() = default;

            void Init();
            void Shutdown();
            void SwapBuffers();

            int32_t VramAvailableKb();
            int32_t VramUsedKb();

        private:
            Window *m_WindowHandle;
            std::string m_Vendor;
            int32_t m_VramAvailableKb = -1;
    };
}
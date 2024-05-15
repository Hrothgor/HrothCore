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

        private:
            Window *m_WindowHandle;
    };
}
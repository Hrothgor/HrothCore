#pragma once

namespace HrothCore
{
    class Renderer
    {
        HC_SINGLETON(Renderer)

        enum class RenderMode
        {
            RENDER_3D,
            RENDER_2D,
            NONE
        };

        public:
            virtual ~Renderer() = default;

            void Init();
            void Shutdown();

            void BeginDrawing3D(int cameraPersp);
            void DrawItem(int data3d);
            void Submit(int data3d);
            void MultiSubmit(std::vector<int> data3d);
            void EndDrawing3D();

            void BeginDrawing2D(int cameraOrtho);
            void DrawItem(float data2d);
            void Submit(float data2d);
            void MultiSubmit(std::vector<float> data2d);
            void EndDrawing2D();
    
        private:
            RenderMode m_CurrentRenderMode = RenderMode::NONE;
    };
}
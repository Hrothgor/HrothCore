#pragma once

namespace HrothCore
{
    struct PerFrameData {
        glm::mat4 mvp;
        int isWireframe;
    };

    class VertexArray;
    template <typename T> class Buffer;
    class Shader;

    class Renderer
    {
        HC_SINGLETON(Renderer)

    public:
        virtual ~Renderer() = default;

        void Init();
        void Shutdown();
        void RenderScene(double dt);

    private:
        std::shared_ptr<VertexArray> m_VAO;
        std::shared_ptr<Buffer<PerFrameData>> m_BufferFrameData;
        std::shared_ptr<Shader> m_BasicShader;

        glm::ivec2 m_FramebufferSize;
    };
}
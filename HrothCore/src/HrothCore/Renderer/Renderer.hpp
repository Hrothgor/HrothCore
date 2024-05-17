#pragma once

namespace HrothCore
{
    class VertexArray;
    class Shader;

    class Renderer
    {
        HC_SINGLETON(Renderer)

    public:
        virtual ~Renderer() = default;

        void Init();
        void Shutdown();
        void RenderScene();

    private:
        std::shared_ptr<VertexArray> m_VAO;
        std::shared_ptr<Shader> m_BasicShader;

        glm::ivec2 m_FramebufferSize;
    };
}
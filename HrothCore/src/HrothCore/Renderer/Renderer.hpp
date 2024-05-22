#pragma once

namespace HrothCore
{
    struct PerFrameData {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
        int isWireframe;
    };

    class VertexArray;
    template <typename T> class Buffer;
    class Shader;
    class Camera;
    class CameraPositionerEditor;

    class Renderer
    {
        HC_SINGLETON(Renderer)

    public:
        virtual ~Renderer() = default;

        void Init();
        void Shutdown();
        void RenderScene(float dt);

        std::shared_ptr<VertexArray> GetVao() const { return m_VAO; }

    private:
        std::shared_ptr<VertexArray> m_VAO;
        std::shared_ptr<Buffer<PerFrameData>> m_BufferFrameData;
        std::shared_ptr<Shader> m_BasicShader;

        std::shared_ptr<Camera> m_Camera;
        std::shared_ptr<CameraPositionerEditor> m_CameraPositionerEditor;

        glm::ivec2 m_FramebufferSize;
    };
}
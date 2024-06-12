#pragma once

#include "HrothCore/Types/Mesh.hpp"

namespace HrothCore
{
    class Camera;
    class Texture;
    class Shader;
    class Framebuffer;

    enum Shaders
    {
        GBufferShader = 0,
        ScreenViewShader,
        NumShaders
    };

    enum Framebuffers
    {
        GBuffer = 0,
        ScreenView,
        NumFramebuffers
    };

    namespace Renderer
    {
        void Init();
        void Shutdown();

        /* Render */

        void BeginScene(const Camera &camera);
        void EndScene();
        void RenderToScreen();

        void DrawMesh(const Mesh &mesh, const Material &material, const glm::mat4 &transform = glm::mat4(1.0f));
        
        /* ------ */

        /* GPU LOADING */
        
        // return Mesh base vertex and index
        std::pair<int, int> LoadVertexData(VerticesData vertices, std::vector<uint32_t> indices);
        uint32_t LoadBindlessTexture(const Texture &texture);

        /* ----------- */

        /* Utils */

        std::shared_ptr<Framebuffer> GetFramebuffer(Framebuffers framebuffer);
        void ResizeView(uint32_t width, uint32_t height);

        std::shared_ptr<Shader> GetShader(Shaders shader);
        void ReloadShader(Shaders shader);
        void ReloadAllShader();

        /* ----- */
    };
}
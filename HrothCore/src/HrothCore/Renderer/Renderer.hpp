#pragma once

#include "HrothCore/Types/Mesh.hpp"

namespace HrothCore
{
    class Camera;
    class Texture;

    enum Shaders
    {
        MeshShader = 0,
        NumShaders
    };

    namespace Renderer
    {
        void Init();
        void Shutdown();

        /* Render */

        void BeginScene(const Camera &camera);
        void EndScene();

        void DrawMesh(const Mesh &mesh, const glm::mat4 &transform = glm::mat4(1.0f));
        
        /* ------ */

        /* GPU LOADING */
        
        // return Mesh base vertex and index
        std::pair<int, int> LoadVertexData(VerticesData vertices, std::vector<uint32_t> indices);
        void LoadBindlessTexture(const Texture &texture);

        /* ----------- */

        /* Utils */

        void ReloadShader(Shaders shader);
        void ReloadAllShader();

        /* ----- */
    };
}
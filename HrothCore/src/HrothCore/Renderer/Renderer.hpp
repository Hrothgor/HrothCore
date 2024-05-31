#pragma once

#include "HrothCore/Types/Mesh.hpp"

namespace HrothCore
{
    class Camera;
    class Texture;

    class Renderer
    {
        public:
            static void Init();
            static void Shutdown();

            static void BeginScene(const Camera &camera);
            static void EndScene();

            static void DrawMesh(const Mesh &mesh, const glm::mat4 &transform = glm::mat4(1.0f));

            // return Mesh base vertex and index
            static std::pair<int, int> LoadVertexData(VerticesData vertices, std::vector<uint32_t> indices);
            static void LoadBindlessTexture(const Texture &texture);
        private:
            static void StartBatch();
            static void NextBatch();
            static void Flush();
    };
}
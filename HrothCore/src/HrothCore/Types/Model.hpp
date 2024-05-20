#pragma once

#include "HrothCore/Types/Mesh.hpp"

#include "HrothCore/Renderer/Texture.hpp"
#include "HrothCore/Types/Mesh.hpp"

namespace HrothCore
{
    class Texture;
    struct Mesh;

    class Model
    {
        public:
            Model();
            ~Model();

            void AddMesh(const Mesh &mesh);

            const Mesh& GetMesh(size_t index) const { return m_Meshes[index]; }
            const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
            size_t GetMeshesCount() const;
        private:
            std::vector<Mesh> m_Meshes;
    };
}
#pragma once

#include "HrothCore/Types/Mesh.hpp"

namespace HrothCore
{
    struct Mesh;

    class Model
    {
        public:
            Model();
            ~Model();

            void AddMesh(const Mesh &mesh);

            const Mesh& GetMesh(size_t index) const
            { 
                HC_ASSERT(index < m_Meshes.size(), "Index out of bounds");
                return m_Meshes[index];
            }
            const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
            size_t GetMeshesCount() const;
        private:
            std::vector<Mesh> m_Meshes;
    };
}
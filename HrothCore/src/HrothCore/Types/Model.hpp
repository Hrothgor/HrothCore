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
            void AddMaterial(const Material &material);

            const Mesh& GetMesh(size_t index) const;
            const Material& GetMaterial(size_t index) const;
        
            size_t GetMeshesCount() const;
            const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
            const std::vector<Material>& GetMaterials() const { return m_Materials; }
        private:
            std::vector<Mesh> m_Meshes;
            std::vector<Material> m_Materials;
    };
}
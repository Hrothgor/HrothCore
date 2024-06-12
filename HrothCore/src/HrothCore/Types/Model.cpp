#include "HrothCore_pch.hpp"

#include "HrothCore/Types/Model.hpp"

namespace HrothCore
{
    Model::Model()
    {
    }

    Model::~Model()
    {
    }

    void Model::AddMesh(const Mesh &mesh)
    {
        m_Meshes.push_back(mesh);
    }

    void Model::AddMaterial(const Material &material)
    {
        m_Materials.push_back(material);
    }

    size_t Model::GetMeshesCount() const
    {
        return m_Meshes.size();
    }

    const Mesh& Model::GetMesh(size_t index) const
    { 
        HC_ASSERT(index < m_Meshes.size(), "Index out of bounds");
        return m_Meshes[index];
    }

    const Material& Model::GetMaterial(size_t index) const
    { 
        HC_ASSERT(index < m_Materials.size(), "Index out of bounds");
        return m_Materials[index];
    }
}
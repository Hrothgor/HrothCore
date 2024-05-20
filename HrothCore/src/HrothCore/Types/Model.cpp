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

    size_t Model::GetMeshesCount() const
    {
        return m_Meshes.size();
    }
}
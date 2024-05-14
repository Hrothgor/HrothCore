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

    void Model::AddMeshIndex(uint32_t index)
    {
        m_MeshIndexes.push_back(index);
    }

    size_t Model::GetMeshesCount() const
    {
        return m_MeshIndexes.size();
    }
}
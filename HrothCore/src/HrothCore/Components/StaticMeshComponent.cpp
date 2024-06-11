#include "HrothCore_pch.hpp"

#include "HrothCore/Components/StaticMeshComponent.hpp"

namespace HrothCore
{
    StaticMeshComponent::StaticMeshComponent()
        : ModelRef()
    {
    }

    StaticMeshComponent::StaticMeshComponent(AssetRef<Model> modelRef)
        : ModelRef(modelRef)
    {
    }
}
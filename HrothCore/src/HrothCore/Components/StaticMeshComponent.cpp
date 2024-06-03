#include "HrothCore_pch.hpp"

#include "HrothCore/Components/StaticMeshComponent.hpp"

namespace HrothCore
{
    StaticMeshComponent::StaticMeshComponent()
        : ModelRef(0)
    {
    }

    StaticMeshComponent::StaticMeshComponent(AssetRef<Model> modelRef)
        : ModelRef(modelRef)
    {
    }
}
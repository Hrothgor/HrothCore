#pragma once

#include "HrothCore/Core/AssetManager.hpp"

namespace HrothCore
{
    struct StaticMeshComponent
    {
        public:
            AssetRef<Model> ModelRef;
            // AssetRef<Material> Material;

            StaticMeshComponent();
            StaticMeshComponent(AssetRef<Model> modelRef);
    };
}
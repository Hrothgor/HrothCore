#pragma once

#include "HrothCore/Types/Mesh.hpp"

namespace HrothCore
{
    class AssetManager
    {
        HC_SINGLETON(AssetManager);

        public:
            virtual ~AssetManager() = default;

        private:
            std::vector<Mesh> m_Meshes;
    };
}
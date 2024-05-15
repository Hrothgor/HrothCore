#pragma once

#include "HrothCore/Types/Mesh.hpp"

namespace HrothCore
{
    class AssetManager
    {
        HC_SINGLETON(AssetManager);

        public:
            virtual ~AssetManager() = default;

            uint32_t GetMeshIndexByPath(const std::string& path);

        private:
            std::vector<Mesh> m_Meshes;
    };
}
#pragma once

#include "HrothCore/Types/Mesh.hpp"
#include "HrothCore/Renderer/Texture.hpp"

namespace HrothCore
{
    template<typename T>
    struct AssetRef
    {
        class AssetManager;
        
        uint32_t Index;

        T Get() const
        {
            return AssetManager::Get().GetAsset<T>(Index);
        }
    };

    class AssetManager
    {
        HC_SINGLETON(AssetManager);

        public:
            virtual ~AssetManager() = default;

            AssetRef<Mesh> GetMeshRef(const std::string& path);
            AssetRef<Texture> GetTextureRef(const std::string& path);

            template<typename T>
            T GetAsset(uint32_t index);

        private:
            std::vector<std::pair<std::string, Mesh>> m_Meshes;
            std::vector<std::pair<std::string, Texture>> m_Textures;
    };
}
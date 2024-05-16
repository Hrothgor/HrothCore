#pragma once

#include "HrothCore/Types/Mesh.hpp"
#include "HrothCore/Renderer/Texture.hpp"

namespace HrothCore
{
    template<typename>
    struct AssetRef;

    class AssetManager
    {
        HC_SINGLETON(AssetManager);

        public:
            virtual ~AssetManager() = default;

            AssetRef<Mesh> GetMeshRef(const std::string& path);
            AssetRef<Texture> GetTextureRef(const std::string& path);

        private:
            template<typename T>
            T& GetAsset(uint32_t index);

            std::vector<std::pair<std::string, Mesh>> m_Meshes;
            std::vector<std::pair<std::string, Texture>> m_Textures;

            friend struct AssetRef<Mesh>;
            friend struct AssetRef<Texture>;
    };

    template<typename T>
    struct AssetRef
    {
        T& Get() const
        {
            return AssetManager::Get().GetAsset<T>(Index);
        }

        AssetRef(uint32_t index)
            : Index(index) {}
            
        private:
            uint32_t Index;
    };
}
#pragma once

#include "HrothCore/Types/Mesh.hpp"
#include "HrothCore/Renderer/Texture.hpp"

namespace HrothCore
{
    template<typename T>
    struct AssetRef
    {
        class AssetManager { private: T& GetAsset(uint32_t index); }; // FORWARD DECLARATION

        T& Get() const
        {
            return AssetManager::Get().GetAsset<T>(Index);
        }

        AssetRef(uint32_t index)
            : Index(index) {}
            
        private:
            uint32_t Index;
    };

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
}
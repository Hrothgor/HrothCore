#pragma once

#include "HrothCore/Types/Model.hpp"
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

            AssetRef<Model> GetModelRef(const std::string &path);
            AssetRef<Texture> GetTextureRef(const std::string &path);

        private:
            template<typename T>
            T& GetAsset(uint32_t index);

            std::vector<std::pair<std::string, Model>> m_Models;
            std::vector<std::pair<std::string, Texture>> m_Textures;

            friend struct AssetRef<Model>;
            friend struct AssetRef<Texture>;
    };

    template<typename T>
    struct AssetRef
    {
        T& Get() const
        {
            return AssetManager::Get().GetAsset<T>(Index);
        }

        bool IsValid() const
        {
            return Index != -1;
        }

        AssetRef(int32_t index = -1)
            : Index(index) {}

        private:
            int32_t Index;
    };
}
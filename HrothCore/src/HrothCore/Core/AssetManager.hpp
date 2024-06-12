#pragma once

#include "HrothCore/Types/Model.hpp"
#include "HrothCore/Renderer/Texture.hpp"

namespace HrothCore
{
    template<typename>
    struct AssetRef;

    namespace AssetManager
    {
        void Init();
        void Shutdown();

        AssetRef<Model> GetModelRef(const std::string &path);
        AssetRef<Texture> GetTextureRef(const std::string &path);
        AssetRef<Material> GetMaterialRef(const std::string &path);

        template<typename T>
        const T& GetAsset(uint32_t index);
    };

    template<typename T>
    struct AssetRef
    {
        const T& Get() const
        {
            return AssetManager::GetAsset<T>(Index);
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
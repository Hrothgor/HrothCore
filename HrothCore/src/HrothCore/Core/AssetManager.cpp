#include "HrothCore_pch.hpp"

#include "HrothCore/Core/AssetManager.hpp"
#include "HrothCore/Core/AssetLoader.hpp"

namespace HrothCore
{
    struct AssetManagerData
    {
        std::vector<std::pair<std::string, Model>> Models;
        std::vector<std::pair<std::string, Texture>> Textures;
    };

    static AssetManagerData s_Data;

    void AssetManager::Init()
    {
    }

    void AssetManager::Shutdown()
    {
        for (auto &texture : s_Data.Textures)
        {
            texture.second.Release();
        }
    }

    AssetRef<Model> AssetManager::GetModelRef(const std::string &path)
    {
        auto it = std::find_if(s_Data.Models.begin(), s_Data.Models.end(), 
            [path](const std::pair<std::string, Model> &modelAsset) { return modelAsset.first == path; });

        if (it == s_Data.Models.end())
        {
            ModelData modelData = AssetLoader::LoadModel(path); // Load file
            Model model;
            for (const MeshData &meshData : modelData.Meshes) {
                model.AddMesh(AssetLoader::LoadMeshToGPU(meshData)); // Load mesh to GPU
            }
            s_Data.Models.push_back({path, model});
            return AssetRef<Model>(static_cast<uint32_t>(s_Data.Models.size() - 1));
        }

        return AssetRef<Model>(static_cast<uint32_t>(it - s_Data.Models.begin()));
    }

    AssetRef<Texture> AssetManager::GetTextureRef(const std::string &path)
    {
        auto it = std::find_if(s_Data.Textures.begin(), s_Data.Textures.end(),
            [path](const std::pair<std::string, Texture> &textureAsset) { return textureAsset.first == path; });

        if (it == s_Data.Textures.end())
        {
            TextureData textureData = AssetLoader::LoadTexture(path); // Load file
            s_Data.Textures.push_back({path, AssetLoader::LoadTextureToGPU(textureData)}); // Load texture to GPU
            return AssetRef<Texture>(static_cast<uint32_t>(s_Data.Textures.size() - 1));
        }

        return AssetRef<Texture>(static_cast<uint32_t>(it - s_Data.Textures.begin()));
    }

    /* ----- GETTER ----- */

    template<>
    const Model& AssetManager::GetAsset<Model>(uint32_t index)
    {
        HC_ASSERT(index < s_Data.Models.size() || index < 0, "Index out of bounds!");
        return s_Data.Models[index].second;
    }

    template<>
    const Texture& AssetManager::GetAsset<Texture>(uint32_t index)
    {
        HC_ASSERT(index < s_Data.Textures.size() || index < 0, "Index out of bounds!");
        return s_Data.Textures[index].second;
    }
}
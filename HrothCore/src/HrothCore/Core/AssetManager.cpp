#include "HrothCore_pch.hpp"

#include "HrothCore/Core/AssetManager.hpp"
#include "HrothCore/Core/AssetLoader.hpp"

namespace HrothCore
{
    struct AssetManagerData
    {
        std::vector<std::pair<std::string, Model>> Models;
        std::vector<std::pair<std::string, Texture>> Textures;
        std::vector<std::pair<std::string, Material>> Materials;
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
            std::string directory = path.substr(0, path.find_last_of('/') + 1);
            for (MaterialData &materialData : modelData.Materials) {
                for (int i = 0; i < MaterialData::TextureType::NumTypes; i++)
                    if (!materialData.Textures[i].empty())
                        materialData.Textures[i] = directory + materialData.Textures[i];
                model.AddMaterial(AssetLoader::LoadMaterialToGPU(materialData)); // Load Material to GPU
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

    AssetRef<Material> AssetManager::GetMaterialRef(const std::string &path)
    {
        auto it = std::find_if(s_Data.Materials.begin(), s_Data.Materials.end(),
            [path](const std::pair<std::string, Material> &materialAsset) { return materialAsset.first == path; });

        // if (it == s_Data.Materials.end())
        // {
        //     Material material; // TODO
        //     s_Data.Materials.push_back({path, material}); // Load texture to GPU
        //     return AssetRef<Material>(static_cast<uint32_t>(s_Data.Materials.size() - 1));
        // }

        return AssetRef<Material>(static_cast<uint32_t>(it - s_Data.Materials.begin()));
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

    template<>
    const Material& AssetManager::GetAsset<Material>(uint32_t index)
    {
        HC_ASSERT(index < s_Data.Materials.size() || index < 0, "Index out of bounds!");
        return s_Data.Materials[index].second;
    }
}
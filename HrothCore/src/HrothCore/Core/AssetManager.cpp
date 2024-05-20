#include "HrothCore_pch.hpp"

#include "HrothCore/Core/AssetManager.hpp"
#include "HrothCore/Core/AssetLoader.hpp"

namespace HrothCore
{
    AssetRef<Model> AssetManager::GetModelRef(const std::string &path)
    {
        auto it = std::find_if(m_Models.begin(), m_Models.end(), 
            [path](const std::pair<std::string, Model> &modelAsset) { return modelAsset.first == path; });

        if (it == m_Models.end())
        {
            std::vector<MeshData> modelData = AssetLoader::LoadModel(path); // Load file
            Model model;
            for (const MeshData &meshData : modelData) {
                model.AddMesh(AssetLoader::LoadMeshToGPU(meshData)); // Load mesh to GPU
            }
            m_Models.push_back({path, model});
            return AssetRef<Model>(static_cast<uint32_t>(m_Models.size() - 1));
        }

        return AssetRef<Model>(static_cast<uint32_t>(it - m_Models.begin()));
    }

    AssetRef<Texture> AssetManager::GetTextureRef(const std::string &path)
    {
        auto it = std::find_if(m_Textures.begin(), m_Textures.end(),
            [path](const std::pair<std::string, Texture> &textureAsset) { return textureAsset.first == path; });

        if (it == m_Textures.end())
        {
            TextureData textureData = AssetLoader::LoadTexture(path); // Load file
            m_Textures.push_back({path, AssetLoader::LoadTextureToGPU(textureData)}); // Load texture to GPU
            return AssetRef<Texture>(static_cast<uint32_t>(m_Textures.size() - 1));
        }

        return AssetRef<Texture>(static_cast<uint32_t>(it - m_Textures.begin()));
    }

    /* ----- GETTER ----- */

    template<>
    Model& AssetManager::GetAsset<Model>(uint32_t index)
    {
        HC_ASSERT(index < m_Models.size());
        return m_Models[index].second;
    }

    template<>
    Texture& AssetManager::GetAsset<Texture>(uint32_t index)
    {
        HC_ASSERT(index < m_Textures.size());
        return m_Textures[index].second;
    }
}
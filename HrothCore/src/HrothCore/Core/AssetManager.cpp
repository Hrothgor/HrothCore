#include "HrothCore_pch.hpp"

#include "HrothCore/Core/AssetManager.hpp"
#include "HrothCore/Core/AssetLoader.hpp"

namespace HrothCore
{
    AssetRef<Mesh> AssetManager::GetMeshRef(const std::string &path)
    {
        auto it = std::find_if(m_Meshes.begin(), m_Meshes.end(), 
            [path](const std::pair<std::string, Mesh> &meshAsset) { return meshAsset.first == path; });

        if (it == m_Meshes.end())
        {
            std::vector<MeshData> modelData = AssetLoader::LoadModel(path); // Load file
            for (const MeshData &meshData : modelData)
                m_Meshes.push_back({path, AssetLoader::LoadMeshToGPU(meshData)}); // Load mesh to GPU
            return AssetRef<Mesh>(static_cast<uint32_t>(m_Meshes.size() - 1));
        }

        return AssetRef<Mesh>(static_cast<uint32_t>(it - m_Meshes.begin()));
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
    Mesh& AssetManager::GetAsset<Mesh>(uint32_t index)
    {
        HC_ASSERT(index < m_Meshes.size());
        return m_Meshes[index].second;
    }

    template<>
    Texture& AssetManager::GetAsset<Texture>(uint32_t index)
    {
        HC_ASSERT(index < m_Textures.size());
        return m_Textures[index].second;
    }
}
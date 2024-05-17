#include "HrothCore_pch.hpp"

#include "HrothCore/Core/AssetLoader.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>

namespace HrothCore
{
    /* ----- Model ----- */

    TextureData ProcessMaterial(aiMaterial *material, aiTextureType type)
    {
        TextureData texture;

        if (material->GetTextureCount(type) > 0)
        {
            aiString str;
            material->GetTexture(type, 0, &str);
            texture.Path = str.C_Str();
        }
        else
            texture.Path = "";

        return texture;
    }

    MeshData ProcessMesh(aiMesh *mesh, const aiScene *scene)
    {
        MeshData meshData;
        
        meshData.Vertices.reserve(mesh->mNumVertices);
        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            if(mesh->mTextureCoords[0])
                vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            meshData.Vertices.push_back(vertex);
        }

        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            meshData.Indices.reserve(face.mNumIndices);
            for(uint32_t j = 0; j < face.mNumIndices; j++)
                meshData.Indices.push_back(face.mIndices[j]);
        }

        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            meshData.Textures[TextureData::Type::Albedo] = ProcessMaterial(material, aiTextureType_DIFFUSE);
            meshData.Textures[TextureData::Type::Metallic] = ProcessMaterial(material, aiTextureType_SPECULAR);
            meshData.Textures[TextureData::Type::Normal] = ProcessMaterial(material, aiTextureType_NORMALS);
            meshData.Textures[TextureData::Type::Height] = ProcessMaterial(material, aiTextureType_HEIGHT);
            meshData.Textures[TextureData::Type::Occlusion] = ProcessMaterial(material, aiTextureType_AMBIENT);
            meshData.Textures[TextureData::Type::Emissive] = ProcessMaterial(material, aiTextureType_EMISSIVE);
        }

        return meshData;
    }  

    void ProcessNode(aiNode *node, const aiScene *scene, std::vector<MeshData> &meshes)
    {
        // process all the node's meshes (if any)
        meshes.reserve(scene->mNumMeshes);
        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene));
        }
        // then do the same for each of its children
        for (uint32_t i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene, meshes);
        }
    }
    
    std::vector<MeshData> AssetLoader::LoadModel(const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            HC_LOG_WARNING("Failed to load model: {0}", path);
            return std::vector<MeshData>();
        }

        std::vector<MeshData> meshes;
        ProcessNode(scene->mRootNode, scene, meshes);

        HC_LOG_INFO("Model loaded: {0}", path);

        return meshes;
    }

    /* ----------------- */

    /* ----- Texture ----- */

    TextureData AssetLoader::LoadTexture(const std::string& path)
    {
        TextureData texture;

        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        texture.Data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!texture.Data)
        {
            HC_LOG_WARNING("Failed to load texture: {0}", path);
            return texture;
        }
        
        texture.Path = path;
        texture.Width = width;
        texture.Height = height;
        texture.Channels = channels;

        HC_LOG_INFO("Texture loaded: {0}", path);

        return texture;
    }

    /* ------------------- */
}
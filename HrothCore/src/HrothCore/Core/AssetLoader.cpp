#include "HrothCore_pch.hpp"

#include "HrothCore/Core/AssetLoader.hpp"
#include "HrothCore/Renderer/Renderer.hpp"
#include "HrothCore/Renderer/VertexArray.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <meshoptimizer.h>

#include <stb_image.h>

namespace HrothCore
{
    /* ----- Model ----- */

    std::string ProcessMaterial(aiMaterial *material, aiTextureType type)
    {
        if (material->GetTextureCount(type) > 0)
        {
            aiString str;
            material->GetTexture(type, 0, &str);
            return std::string(str.C_Str());
        }
        else
            return std::string();
    }

    MeshData ProcessMesh(aiMesh *mesh, const aiScene *scene)
    {
        MeshData meshData;
        
        meshData.Vertices.Position.reserve(mesh->mNumVertices);
        meshData.Vertices.Normal.reserve(mesh->mNumVertices);
        meshData.Vertices.TexCoords.reserve(mesh->mNumVertices);
        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            meshData.Vertices.Position.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
            if (mesh->HasNormals())
                meshData.Vertices.Normal.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
            else 
                meshData.Vertices.Normal.push_back(glm::vec3(0.0f));
            if (mesh->HasTextureCoords(0))
                meshData.Vertices.TexCoords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
            else
                meshData.Vertices.TexCoords.push_back(glm::vec2(0.0f));
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
            meshData.Textures[MeshData::TextureType::Albedo] = ProcessMaterial(material, aiTextureType_DIFFUSE);
            meshData.Textures[MeshData::TextureType::Metallic] = ProcessMaterial(material, aiTextureType_SPECULAR);
            meshData.Textures[MeshData::TextureType::Normal] = ProcessMaterial(material, aiTextureType_NORMALS);
            meshData.Textures[MeshData::TextureType::Height] = ProcessMaterial(material, aiTextureType_HEIGHT);
            meshData.Textures[MeshData::TextureType::Occlusion] = ProcessMaterial(material, aiTextureType_AMBIENT);
            meshData.Textures[MeshData::TextureType::Emissive] = ProcessMaterial(material, aiTextureType_EMISSIVE);
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

    void OptimizeMesh(std::vector<MeshData> &meshes)
    {
        for (MeshData &mesh : meshes)
        {
            std::vector<unsigned int> remap(mesh.Indices.size());
            meshopt_Stream streams[] = {
                {mesh.Vertices.Position.data(), sizeof(glm::vec3), sizeof(glm::vec3)},
                {mesh.Vertices.Normal.data(), sizeof(glm::vec3), sizeof(glm::vec3)},
                {mesh.Vertices.TexCoords.data(), sizeof(glm::vec2), sizeof(glm::vec2)},
            };
            const size_t vertexCount = meshopt_generateVertexRemapMulti(remap.data(), mesh.Indices.data(), mesh.Indices.size(),
                                                mesh.Vertices.Position.size(), streams, sizeof(streams) / sizeof(streams[0]));

            std::vector<unsigned int> remappedIndices(mesh.Indices.size());
            std::vector<glm::vec3> remappedPos(vertexCount);
            std::vector<glm::vec3> remappedNormal(vertexCount);
            std::vector<glm::vec2> remappedTexCoords(vertexCount);

            meshopt_remapIndexBuffer(remappedIndices.data(), mesh.Indices.data(), mesh.Indices.size(), remap.data());
            meshopt_remapVertexBuffer(remappedPos.data(), mesh.Vertices.Position.data(), mesh.Vertices.Position.size(), sizeof(glm::vec3), remap.data());
            meshopt_remapVertexBuffer(remappedNormal.data(), mesh.Vertices.Normal.data(), mesh.Vertices.Normal.size(), sizeof(glm::vec3), remap.data());
            meshopt_remapVertexBuffer(remappedTexCoords.data(), mesh.Vertices.TexCoords.data(), mesh.Vertices.TexCoords.size(), sizeof(glm::vec2), remap.data());

            meshopt_optimizeVertexCache(remappedIndices.data(), remappedIndices.data(), remappedIndices.size(), vertexCount);
            meshopt_optimizeOverdraw(remappedIndices.data(), remappedIndices.data(), remappedIndices.size(), glm::value_ptr(remappedPos[0]), vertexCount, sizeof(glm::vec3), 1.05f);

            meshopt_optimizeVertexFetchRemap(remap.data(), remappedIndices.data(), remappedIndices.size(), vertexCount);

            meshopt_remapIndexBuffer(remappedIndices.data(), remappedIndices.data(), remappedIndices.size(), remap.data());
            meshopt_remapVertexBuffer(remappedPos.data(), remappedPos.data(), vertexCount, sizeof(glm::vec3), remap.data());
            meshopt_remapVertexBuffer(remappedNormal.data(), remappedNormal.data(), vertexCount, sizeof(glm::vec3), remap.data());
            meshopt_remapVertexBuffer(remappedTexCoords.data(), remappedTexCoords.data(), vertexCount, sizeof(glm::vec2), remap.data());

            mesh.Indices = remappedIndices;
            mesh.Vertices.Position = remappedPos;
            mesh.Vertices.Normal = remappedNormal;
            mesh.Vertices.TexCoords = remappedTexCoords;
        }

    }
    
    std::vector<MeshData> AssetLoader::LoadModel(const std::string &path)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            HC_LOG_WARNING("Failed to load model: {0}", path);
            importer.FreeScene();
            return std::vector<MeshData>();
        }

        std::vector<MeshData> meshes;
        ProcessNode(scene->mRootNode, scene, meshes);
        importer.FreeScene();

        HC_LOG_INFO("Model loaded: {0}", path);

        OptimizeMesh(meshes);

        HC_LOG_INFO("Model optimzed: {0}", path);

        return meshes;
    }

    /* ----------------- */

    /* ----- Texture ----- */

    TextureData AssetLoader::LoadTexture(const std::string &path)
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
        
        texture.Width = width;
        texture.Height = height;
        texture.Channels = channels;

        HC_LOG_INFO("Texture loaded: {0}", path);

        return texture;
    }

    /* ------------------- */

    /* ----- Load to GPU ----- */

    Mesh AssetLoader::LoadMeshToGPU(const MeshData &meshData)
    {
        Mesh mesh;

        mesh.VerticesCount = static_cast<uint32_t>(meshData.Vertices.Position.size());
        mesh.IndicesCount = static_cast<uint32_t>(meshData.Indices.size());

        mesh.BaseVertex = 0;
        mesh.BaseIndex = 0;

        Renderer::Get().GetVao()->AddVertices(meshData.Vertices, meshData.Indices);

        return mesh;
    }

    Texture AssetLoader::LoadTextureToGPU(const TextureData &textureData)
    {
        TextureInfo info;
        info.wrapMode = TextureInfo::WrapMode::Repeat;
        info.filterMode = TextureInfo::FilterMode::Linear;
        switch (textureData.Channels)
        {
            case 1: info.format = TextureInfo::Format::R; break;
            case 2: info.format = TextureInfo::Format::RG; break;
            case 3: info.format = TextureInfo::Format::RGB; break;
            case 4: info.format = TextureInfo::Format::RGBA; break;
        }
        info.dataType = TextureInfo::DataType::UByte;
        
        Texture texture(textureData.Width, textureData.Height, info);
        texture.SetData(textureData.Data);

        stbi_image_free((void *)textureData.Data);

        return texture;
    }

    /* ----------------------- */
}
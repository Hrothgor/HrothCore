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

    std::string ProcessMaterialTexture(aiMaterial *material, aiTextureType type, int index = 0)
    {
        // Get the path to the texture
        aiString path;
        material->GetTexture(type, index, &path);
        return std::string(path.C_Str());
    }

    MaterialData ProcessMaterial(aiMaterial *material, const aiScene *scene)
    {
        MaterialData materialData;

        materialData.Textures[MaterialData::TextureType::Albedo] = ProcessMaterialTexture(material, aiTextureType_BASE_COLOR);
        materialData.Textures[MaterialData::TextureType::Metallic] = ProcessMaterialTexture(material, aiTextureType_METALNESS);
        materialData.Textures[MaterialData::TextureType::Normal] = ProcessMaterialTexture(material, aiTextureType_NORMALS);
        materialData.Textures[MaterialData::TextureType::Occlusion] = ProcessMaterialTexture(material, aiTextureType_AMBIENT);
        materialData.Textures[MaterialData::TextureType::Emissive] = ProcessMaterialTexture(material, aiTextureType_EMISSIVE);

        materialData.AlbedoValue = glm::vec3(1.0f);
        // Retrieve the diffuse color (albedo)
        aiColor3D diffuseColor;
        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
            materialData.AlbedoValue = glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);

        return materialData;
    }

    void ProcessMaterials(const aiScene *scene, ModelData &modelData)
    {
        modelData.Materials.reserve(scene->mNumMaterials);
        for (uint32_t i = 0; i < scene->mNumMaterials; i++)
        {
            aiMaterial *material = scene->mMaterials[i];
            modelData.Materials.push_back(ProcessMaterial(material, scene));
        }
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

        meshData.MaterialIndex = mesh->mMaterialIndex;

        return meshData;
    }  

    void ProcessMeshNode(aiNode *node, const aiScene *scene, ModelData &modelData)
    {
        // process all the node's meshes (if any)
        modelData.Meshes.reserve(scene->mNumMeshes);
        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            modelData.Meshes.push_back(ProcessMesh(mesh, scene));
        }
        // then do the same for each of its children
        for (uint32_t i = 0; i < node->mNumChildren; i++)
        {
            ProcessMeshNode(node->mChildren[i], scene, modelData);
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
    
    ModelData AssetLoader::LoadModel(const std::string &path)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            HC_LOG_WARNING("Failed to load model: {0}", path);
            importer.FreeScene();
            return ModelData();
        }

        ModelData modelData;
        ProcessMeshNode(scene->mRootNode, scene, modelData);
        ProcessMaterials(scene, modelData);
        importer.FreeScene();

        HC_LOG_INFO("Model loaded: {0}", path);

        OptimizeMesh(modelData.Meshes);

        HC_LOG_INFO("Model optimzed: {0}", path);

        return modelData;
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

        std::pair<int, int> offsets = Renderer::LoadVertexData(meshData.Vertices, meshData.Indices);

        mesh.VerticesCount = static_cast<uint32_t>(meshData.Vertices.Position.size());
        mesh.IndicesCount = static_cast<uint32_t>(meshData.Indices.size());

        mesh.BaseVertex = offsets.first;
        mesh.BaseIndex = offsets.second;

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

        Renderer::LoadBindlessTexture(texture);

        return texture;
    }

    /* ----------------------- */
}
#pragma once

#include "HrothCore/Types/Mesh.hpp"
#include "HrothCore/Renderer/Texture.hpp"

namespace HrothCore
{
    struct VerticesData
    {
        std::vector<glm::vec3> Position;
        std::vector<glm::vec3> Normal;
        std::vector<glm::vec2> TexCoords;
    };

    struct TextureData
    {
        const uint8_t* Data = nullptr;
        uint32_t Width = 0;
        uint32_t Height = 0;
        uint32_t Channels = 0;
    };

    struct MaterialData
    {
        enum TextureType : uint32_t
        {
            Albedo,
            Metallic,
            Normal,
            Occlusion,
            Emissive,
            NumTypes
        };

        std::array<std::string, TextureType::NumTypes> Textures;

        glm::vec3 AlbedoValue;
        float MetallicValue;
        float AmbientOcclusionValue;
    };

    struct MeshData
    {
        VerticesData Vertices;
        std::vector<uint32_t> Indices;
        uint32_t MaterialIndex;
    };

    struct ModelData
    {
        std::vector<MeshData> Meshes;
        std::vector<MaterialData> Materials;
    };

    class AssetLoader
    {

        public:
            static ModelData LoadModel(const std::string &path);
            static TextureData LoadTexture(const std::string &path);

        private:
            static Mesh LoadMeshToGPU(const MeshData &meshData);
            static Texture LoadTextureToGPU(const TextureData &textureData);

        friend class AssetManager;
    };
}
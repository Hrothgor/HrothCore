#pragma once

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

    struct Mesh
    {
        uint32_t VerticesCount;
        uint32_t IndicesCount;

        uint32_t BaseVertex;
        uint32_t BaseIndex;

        uint32_t MaterialIndex;
    };
}
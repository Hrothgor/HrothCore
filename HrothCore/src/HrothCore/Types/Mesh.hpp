#pragma once

namespace HrothCore
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct TextureData
    {
        const uint8_t* Data = nullptr;
        uint32_t Width = 0;
        uint32_t Height = 0;
        uint32_t Channels = 0;
    };

    struct MeshData
    {
        enum TextureType : uint32_t
        {
            Albedo,
            Metallic,
            Normal,
            Height,
            Occlusion,
            Emissive,
            NumTypes
        };

        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;
        std::array<std::string, TextureType::NumTypes> Textures;
    };

    struct Mesh
    {
        uint32_t VerticesCount;
        uint32_t IndicesCount;

        uint32_t BaseVertex;
        uint32_t BaseIndex;
    };
}
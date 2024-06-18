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
            Diffuse,
            Specular,
            Normal,
            Occlusion,
            Emissive,
            NumTypes
        };
        std::array<std::string, TextureType::NumTypes> Textures;

        float OcclusionStrength = 1.0f; // between 0 and 1
        float EmissiveIntensity = 1.0f;

        glm::vec3 Color = glm::vec3(1.0f);

        float Shininess = 16.0f;
        float Reflectivity = 0.5f; // between 0 and 1
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

    struct Material
    {
        int32_t DiffuseTextureIndex = -1; // index in SamplerBuffer
        int32_t SpecularTextureIndex = -1; // index in SamplerBuffer
        int32_t NormalTextureIndex = -1; // index in SamplerBuffer
        int32_t OcclusionTextureIndex = -1; // index in SamplerBuffer
        int32_t EmissiveTextureIndex = -1; // index in SamplerBuffer

        float OcclusionStrength = 1.0f; // between 0 and 1
        float EmissiveIntensity = 1.0f;

        alignas(16) glm::vec3 Color = glm::vec3(1.0f);

        float Shininess = 16.0f;
        float Reflectivity = 0.5f; // between 0 and 1
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
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

        glm::vec3 DiffuseColor = glm::vec3(1.0f);
        glm::vec3 SpecularColor = glm::vec3(1.0f);
        glm::vec3 AmbientColor = glm::vec3(1.0f);
        glm::vec3 EmissiveColor = glm::vec3(1.0f);

        float OcclusionStrength = 1.0f;

        float Shininess = 32.0f;
        float DiffuseReflectivity = 1.0f;
        float SpecularReflectivity = 1.0f;
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
        int32_t DiffuseTextureIndex; // index in SamplerBuffer
        int32_t SpecularTextureIndex; // index in SamplerBuffer
        int32_t NormalTextureIndex; // index in SamplerBuffer
        int32_t OcclusionTextureIndex; // index in SamplerBuffer
        int32_t EmissiveTextureIndex; // index in SamplerBuffer

        alignas(16) glm::vec3 DiffuseColor;
        alignas(16) glm::vec3 SpecularColor;
        alignas(16) glm::vec3 AmbientColor;
        alignas(16) glm::vec3 EmissiveColor;

        float OcclusionStrength;

        float Shininess;
        float DiffuseReflectivity;
        float SpecularReflectivity;
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
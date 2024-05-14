#pragma once

#include "HrothCore/Types/Model.hpp"

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
        enum Type : uint32_t
        {
            Albedo,
            Metallic,
            Normal,
            Height,
            Occlusion,
            Emissive,
            NumTypes
        };

        std::string Path;
    };

    struct MeshData
    {
        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;
        std::array<TextureData, TextureData::Type::NumTypes> Textures;
    };

    class AssetLoader
    {
        static std::vector<MeshData> LoadModel(const std::string& path);
    };
}
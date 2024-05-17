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
        const uint8_t* Data = nullptr;
        uint32_t Width = 0;
        uint32_t Height = 0;
        uint32_t Channels = 0;
    };

    struct MeshData
    {
        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;
        std::array<TextureData, TextureData::Type::NumTypes> Textures;
    };

    class AssetLoader
    {
        public:
            static std::vector<MeshData> LoadModel(const std::string& path);
            static TextureData LoadTexture(const std::string& path);

        private:
    };
}
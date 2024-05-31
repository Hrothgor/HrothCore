#pragma once

#include "HrothCore/Types/Mesh.hpp"
#include "HrothCore/Renderer/Texture.hpp"

namespace HrothCore
{
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
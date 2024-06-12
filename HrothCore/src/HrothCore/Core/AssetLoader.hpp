#pragma once

#include "HrothCore/Types/Mesh.hpp"
#include "HrothCore/Renderer/Texture.hpp"

namespace HrothCore
{
    namespace AssetLoader
    {
        ModelData LoadModel(const std::string &path);
        TextureData LoadTexture(const std::string &path);

        Mesh LoadMeshToGPU(const MeshData &meshData);
        Material LoadMaterialToGPU(const MaterialData &materialData);
        Texture LoadTextureToGPU(const TextureData &textureData);
    };
}
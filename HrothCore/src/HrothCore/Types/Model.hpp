#pragma once

#include "HrothCore/Types/Mesh.hpp"

#include "HrothCore/Renderer/Texture.hpp"
#include "HrothCore/Types/Mesh.hpp"

namespace HrothCore
{
    class Texture;
    struct Mesh;

    class Model
    {
        public:
            Model();
            ~Model();

            void AddMeshIndex(uint32_t index);
            size_t GetMeshesCount() const;

        private:
            std::vector<uint32_t> m_MeshIndexes;
    };
}
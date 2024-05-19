#pragma once

#include "HrothCore/Renderer/Buffer.hpp"
#include "HrothCore/Types/Mesh.hpp"

namespace HrothCore
{
    class VertexArray
    {
        public:
            VertexArray(bool indexed = true);
            ~VertexArray();

            void Bind() const;

            void AddVertices(VerticesData vertices, std::vector<uint32_t> indices = {});

            uint32_t GetVerticesCount() const { return m_VerticesCount; }
        private:
            uint32_t m_VaoID;

            Buffer<glm::vec3> m_VboPosition;
            Buffer<glm::vec3> m_VboNormal;
            Buffer<glm::vec2> m_VboTexCoords;
            Buffer<uint32_t> m_Ibo;

            bool m_Indexed;
            uint32_t m_VerticesCount;
    };
}
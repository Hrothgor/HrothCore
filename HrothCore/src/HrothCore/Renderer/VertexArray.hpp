#pragma once

#include "HrothCore/Renderer/Buffer.hpp"
#include "HrothCore/Types/Mesh.hpp"

namespace HrothCore
{
    class VertexArray
    {
        public:
            VertexArray(bool indexed = true);
            VertexArray(std::vector<Vertex> vertices, std::vector<uint32_t> indices = {});
            ~VertexArray();

            void Bind() const;

            void AddVertices(std::vector<Vertex> vertices, std::vector<uint32_t> indices = {});

            uint32_t GetVerticesCount() const { return m_VerticesCount; }
        private:
            uint32_t m_VaoID;
            Buffer<Vertex> m_Vbo;
            Buffer<uint32_t> m_Ibo;

            bool m_Indexed;
            uint32_t m_VerticesCount;
    };
}
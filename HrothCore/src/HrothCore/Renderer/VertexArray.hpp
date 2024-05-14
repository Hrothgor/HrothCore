#pragma once

#include "HrothCore/Renderer/Buffer.hpp"

namespace HrothCore
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    class VertexArray
    {
        public:
            VertexArray(std::vector<Vertex> vertices, std::vector<uint32_t> indices = {});
            ~VertexArray();

            void Bind() const;

            uint32_t GetVerticesCount() const { return m_VerticesCount; }

        private:
            void GenerateVertexArray(std::vector<Vertex> vertices);
            void GenerateVertexArrayIndexed(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

            uint32_t m_VaoID;
            Buffer<Vertex> m_Vbo;
            Buffer<uint32_t> m_Ibo;

            uint32_t m_VerticesCount;
    };
}
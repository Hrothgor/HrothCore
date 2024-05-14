#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/VertexArray.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    VertexArray::VertexArray(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
        : m_VaoID(0)
        , m_Vbo(vertices.data(), static_cast<uint32_t>(vertices.size()), BufferUsage::Dynamic)
        , m_Ibo(indices.data(), static_cast<uint32_t>(indices.size()), BufferUsage::Dynamic)
        , m_VerticesCount(static_cast<uint32_t>(vertices.size()))
    {
        glCreateVertexArrays(1, &m_VaoID);

        glVertexArrayVertexBuffer(m_VaoID, 0, m_Vbo.GetID(), 0, sizeof(Vertex));
    
        if (indices.size() != 0)
        {
            glVertexArrayElementBuffer(m_VaoID, m_Ibo.GetID());
        }

        glEnableVertexArrayAttrib(m_VaoID, 0);
        glEnableVertexArrayAttrib(m_VaoID, 1);
        glEnableVertexArrayAttrib(m_VaoID, 2);

        glVertexArrayAttribFormat(m_VaoID, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position));
        glVertexArrayAttribFormat(m_VaoID, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Normal));
        glVertexArrayAttribFormat(m_VaoID, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TexCoords));

        glVertexArrayAttribBinding(m_VaoID, 0, 0);
        glVertexArrayAttribBinding(m_VaoID, 1, 0);
        glVertexArrayAttribBinding(m_VaoID, 2, 0);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_VaoID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_VaoID);
    }
}
#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/VertexArray.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    VertexArray::VertexArray(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
        : m_VaoID(0), m_VboID(0), m_iboID(0), m_VerticesCount(vertices.size())
    {
        glCreateVertexArrays(1, &m_VaoID);

        glCreateBuffers(1, &m_VboID);
        glNamedBufferStorage(m_VboID, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_STORAGE_BIT);
        glVertexArrayVertexBuffer(m_VaoID, 0, m_VboID, 0, sizeof(Vertex));
    
        if (indices.size() != 0)
        {
            glCreateBuffers(1, &m_iboID);
            glNamedBufferStorage(m_iboID, indices.size() * sizeof(uint32_t), indices.data(), GL_DYNAMIC_STORAGE_BIT);
            glVertexArrayElementBuffer(m_VaoID, m_iboID);
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
        glDeleteBuffers(1, &m_VboID);
        glDeleteBuffers(1, &m_iboID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_VaoID);
    }
}
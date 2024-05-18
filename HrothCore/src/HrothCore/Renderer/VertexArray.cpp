#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/VertexArray.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    VertexArray::VertexArray(bool indexed)
        : m_VaoID(0)
        , m_Vbo(BufferUsage::Dynamic)
        , m_Ibo(BufferUsage::Dynamic)
        , m_VerticesCount(0)
        , m_Indexed(indexed)
    {
        glCreateVertexArrays(1, &m_VaoID);

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

    void VertexArray::AddVertices(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
    {
        if (m_Indexed && indices.empty() || !m_Indexed && !indices.empty())
        {
            HC_LOG_WARNING("VertexArray::AddVertices: vertex array is indexed, but no indices provided or vice versa");
            return;
        }
        
        m_VerticesCount += m_Indexed ? static_cast<uint32_t>(indices.size()) : static_cast<uint32_t>(vertices.size());

        m_Vbo.AddData(static_cast<uint32_t>(vertices.size()), vertices.data());
        if (m_Indexed)
            m_Ibo.AddData(static_cast<uint32_t>(indices.size()), indices.data());

        glVertexArrayVertexBuffer(m_VaoID, 0, m_Vbo.GetID(), 0, sizeof(Vertex));
        if (m_Indexed)
            glVertexArrayElementBuffer(m_VaoID, m_Ibo.GetID());
    }
}
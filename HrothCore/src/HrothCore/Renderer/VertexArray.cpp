#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/VertexArray.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    VertexArray::VertexArray(bool indexed)
        : m_VaoID(0)
        , m_VboPosition(BufferUsage::Dynamic)
        , m_VboNormal(BufferUsage::Dynamic)
        , m_VboTexCoords(BufferUsage::Dynamic)
        , m_Ibo(BufferUsage::Dynamic)
        , m_VerticesCount(0)
        , m_Indexed(indexed)
    {
        glCreateVertexArrays(1, &m_VaoID);

        glEnableVertexArrayAttrib(m_VaoID, 0);
        glEnableVertexArrayAttrib(m_VaoID, 1);
        glEnableVertexArrayAttrib(m_VaoID, 2);

        glVertexArrayAttribFormat(m_VaoID, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(m_VaoID, 1, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(m_VaoID, 2, 2, GL_FLOAT, GL_FALSE, 0);

        glVertexArrayAttribBinding(m_VaoID, 0, 0);
        glVertexArrayAttribBinding(m_VaoID, 1, 1);
        glVertexArrayAttribBinding(m_VaoID, 2, 2);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_VaoID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_VaoID);
    }

    void VertexArray::AddVertices(VerticesData vertices, std::vector<uint32_t> indices)
    {
        if (m_Indexed && indices.empty() || !m_Indexed && !indices.empty())
        {
            HC_LOG_WARNING("VertexArray::AddVertices: vertex array is indexed, but no indices provided or vice versa");
            return;
        }
        
        m_VerticesCount += m_Indexed ? static_cast<uint32_t>(indices.size()) : static_cast<uint32_t>(vertices.Position.size());

        m_VboPosition.AddData(static_cast<uint32_t>(vertices.Position.size()), vertices.Position.data());
        glVertexArrayVertexBuffer(m_VaoID, 0, m_VboPosition.GetID(), 0, sizeof(glm::vec3));

        m_VboNormal.AddData(static_cast<uint32_t>(vertices.Normal.size()), vertices.Normal.data());
        glVertexArrayVertexBuffer(m_VaoID, 1, m_VboNormal.GetID(), 0, sizeof(glm::vec3));

        m_VboTexCoords.AddData(static_cast<uint32_t>(vertices.TexCoords.size()), vertices.TexCoords.data());
        glVertexArrayVertexBuffer(m_VaoID, 2, m_VboTexCoords.GetID(), 0, sizeof(glm::vec2));

        if (m_Indexed)
        {
            m_Ibo.AddData(static_cast<uint32_t>(indices.size()), indices.data());
            glVertexArrayElementBuffer(m_VaoID, m_Ibo.GetID());
        }
    }
}
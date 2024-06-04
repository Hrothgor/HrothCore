#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/VertexArray.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    VertexArray::VertexArray()
        : m_VaoID(0)
        , m_VboPosition(BufferUsage::Dynamic)
        , m_VboNormal(BufferUsage::Dynamic)
        , m_VboTexCoords(BufferUsage::Dynamic)
        , m_Ibo(BufferUsage::Dynamic)
        , m_VerticesCount(0)
        , m_IndicesCount(0)
    {
        glCreateVertexArrays(1, &m_VaoID);

        glEnableVertexArrayAttrib(m_VaoID, 0);
        glEnableVertexArrayAttrib(m_VaoID, 1);
        glEnableVertexArrayAttrib(m_VaoID, 2);

        glVertexArrayAttribFormat(m_VaoID, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(m_VaoID, 1, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(m_VaoID, 2, 2, GL_FLOAT, GL_FALSE, 0);

        glVertexArrayVertexBuffer(m_VaoID, 0, m_VboPosition.GetID(), 0, sizeof(glm::vec3));
        glVertexArrayVertexBuffer(m_VaoID, 1, m_VboNormal.GetID(), 0, sizeof(glm::vec3));
        glVertexArrayVertexBuffer(m_VaoID, 2, m_VboTexCoords.GetID(), 0, sizeof(glm::vec2));
        glVertexArrayElementBuffer(m_VaoID, m_Ibo.GetID());

        glVertexArrayAttribBinding(m_VaoID, 0, 0);
        glVertexArrayAttribBinding(m_VaoID, 1, 1);
        glVertexArrayAttribBinding(m_VaoID, 2, 2);
    }

    VertexArray::~VertexArray()
    {
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_VaoID);
    }

    void VertexArray::AddVertices(VerticesData vertices, std::vector<uint32_t> indices)
    {
        if (indices.empty())
        {
            HC_LOG_WARNING("VertexArray::AddVertices: vertex array is indexed, but no indices provided");
            return;
        }
        
        m_VerticesCount += static_cast<uint32_t>(vertices.Position.size());
        m_IndicesCount += static_cast<uint32_t>(indices.size());

        m_VboPosition.AddData(static_cast<uint32_t>(vertices.Position.size()), vertices.Position.data());
        glVertexArrayVertexBuffer(m_VaoID, 0, m_VboPosition.GetID(), 0, sizeof(glm::vec3));

        m_VboNormal.AddData(static_cast<uint32_t>(vertices.Normal.size()), vertices.Normal.data());
        glVertexArrayVertexBuffer(m_VaoID, 1, m_VboNormal.GetID(), 0, sizeof(glm::vec3));

        m_VboTexCoords.AddData(static_cast<uint32_t>(vertices.TexCoords.size()), vertices.TexCoords.data());
        glVertexArrayVertexBuffer(m_VaoID, 2, m_VboTexCoords.GetID(), 0, sizeof(glm::vec2));

        m_Ibo.AddData(static_cast<uint32_t>(indices.size()), indices.data());
        glVertexArrayElementBuffer(m_VaoID, m_Ibo.GetID());
    }

    void VertexArray::Release()
    {
        glDeleteVertexArrays(1, &m_VaoID);
        m_VboPosition.Release();
        m_VboNormal.Release();
        m_VboTexCoords.Release();
        m_Ibo.Release();
    }
}
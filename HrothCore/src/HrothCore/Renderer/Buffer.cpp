#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Buffer.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    /* ----- BufferUsage to GL -----*/
    GLenum GetGLBufferUsage(BufferUsage usage);
    /* -----------------------------*/

    Buffer::Buffer(BufferUsage usage)
        : m_Usage(usage), m_Capacity(0)
    {
        glCreateBuffers(1, &m_HandleID);
    }

    Buffer::Buffer(uint32_t capacity, BufferUsage usage)
        : m_Usage(usage), m_Capacity(capacity)
    {
        glCreateBuffers(1, &m_HandleID);
        glNamedBufferStorage(m_HandleID, m_Size, nullptr, GetGLBufferUsage(m_Usage));
    }

    Buffer::Buffer(const void *data, uint32_t dataSize, BufferUsage usage)
        : m_Usage(usage), m_Capacity(dataSize), m_Size(dataSize)
    {
        glCreateBuffers(1, &m_HandleID);
        glNamedBufferStorage(m_HandleID, m_Size, data, GetGLBufferUsage(m_Usage));
    }

    Buffer::~Buffer()
    {
        glDeleteBuffers(1, &m_HandleID);
    }

    void Buffer::SetData(const void *data, uint32_t size, uint32_t offset)
    {
        HC_ASSERT(size + offset <= m_Capacity);
        HC_ASSERT(data != nullptr);

        if (size + offset > m_Size)
            m_Size = size + offset;
        glNamedBufferSubData(m_HandleID, offset, size, data);
    }

    void Buffer::AddData(const void *data, uint32_t size)
    {
        if (m_Capacity == 0)
            Resize(size);
        else if (m_Size + size > m_Capacity)
        {
            uint32_t newCapacity = m_Capacity * 2;
            while (m_Size + size > newCapacity)
                newCapacity *= 2;
            Resize(newCapacity);
        }
        SetData(data, size, m_Size);
    }

    void Buffer::Reserve(uint32_t capacity)
    {
        if (capacity > m_Capacity)
            Resize(capacity);
    }

    void Buffer::Resize(uint32_t newCapacity)
    {
        m_Capacity = newCapacity;

        uint32_t newBufferID = 0;
        glCreateBuffers(1, &newBufferID);
        glNamedBufferStorage(newBufferID, newCapacity, nullptr, GetGLBufferUsage(m_Usage));

        glCopyNamedBufferSubData(m_HandleID, newBufferID, 0, 0, m_Size);

        glDeleteBuffers(1, &m_HandleID);
        m_HandleID = newBufferID;
    }

    /* ----- BufferUsage to GL -----*/

    GLenum GetGLBufferUsage(BufferUsage usage)
    {
        switch (usage)
        {
            case BufferUsage::Static: return 0;
            case BufferUsage::Dynamic: return GL_DYNAMIC_STORAGE_BIT;
        }
        return 0;
    }
}
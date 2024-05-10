#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Buffer.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    Buffer::Buffer(uint32_t size, const void *data, BufferUsage usage)
        : m_Size(size), m_Usage(usage), m_HandleID(0)
    {
        GLbitfield usageGL;
        switch (usage)
        {
            case BufferUsage::Static: usageGL = 0;
            case BufferUsage::Dynamic: usageGL |= GL_DYNAMIC_STORAGE_BIT;
        }

        glCreateBuffers(1, &m_HandleID);
        glNamedBufferStorage(m_HandleID, m_Size, data, usageGL);
    }

    Buffer::~Buffer()
    {
        glDeleteBuffers(1, &m_HandleID);
    }

    void Buffer::SetData(uint32_t size, const void *data, uint32_t offset)
    {
        HC_ASSERT(size + offset <= m_Size);
        HC_ASSERT(data != nullptr);

        glNamedBufferSubData(m_HandleID, offset, size, data);
    }
}
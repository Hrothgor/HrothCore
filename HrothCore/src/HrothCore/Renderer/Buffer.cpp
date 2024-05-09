#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Buffer.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    Buffer::Buffer()
        : m_Size(0), m_HandleID(0)
    {
        glCreateBuffers(1, &m_HandleID);
    }

    Buffer::~Buffer()
    {
        glDeleteBuffers(1, &m_HandleID);
    }

    void Buffer::SetData(const void *data, uint32_t size, BufferUsage usage)
    {
        GLenum usageGL = 0;
        switch (usage)
        {
        case BufferUsage::Static: usageGL = GL_STATIC_DRAW;
        case BufferUsage::Dynamic: usageGL = GL_DYNAMIC_DRAW;
        case BufferUsage::Stream: usageGL = GL_STREAM_DRAW;
        }
        glNamedBufferData(m_HandleID, size, data, usageGL);
    }
}
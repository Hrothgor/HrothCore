#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Buffer.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    /* ----- BufferUsage to GL -----*/
    GLenum GetGLBufferUsage(BufferUsage usage);
    /* -----------------------------*/

    template<typename T>
    Buffer<T>::Buffer(BufferUsage usage)
        : m_Usage(usage), m_Capacity(1)
    {
        glCreateBuffers(1, &m_HandleID);
    }

    template<typename T>
    Buffer<T>::Buffer(uint32_t capacity, BufferUsage usage)
        : m_Usage(usage), m_Capacity(capacity)
    {
        glCreateBuffers(1, &m_HandleID);
        glNamedBufferStorage(m_HandleID, m_Capacity * sizeof(T), nullptr, GetGLBufferUsage(m_Usage));
    }

    template<typename T>
    Buffer<T>::Buffer(const T *data, uint32_t dataLength, BufferUsage usage)
        : m_Usage(usage), m_Capacity(dataLength), m_Size(dataLength)
    {
        glCreateBuffers(1, &m_HandleID);
        glNamedBufferStorage(m_HandleID, m_Capacity * sizeof(T), data, GetGLBufferUsage(m_Usage));
    }

    template<typename T>
    Buffer<T>::~Buffer()
    {
        glDeleteBuffers(1, &m_HandleID);
    }

    template<typename T>
    void Buffer<T>::SetData(const T *data, uint32_t length, uint32_t offset)
    {
        if (m_Usage == BufferUsage::Static)
        {
            HC_LOG_WARNING("Buffer::SetData: cannot modify data to static buffer");
            return;
        }

        HC_ASSERT(length + offset <= m_Capacity);

        HC_LOG_WARNING("Buffer::SetData: data is nullptr");

        if (length + offset > m_Size)
            m_Size = length + offset;
        glNamedBufferSubData(m_HandleID, offset * sizeof(T), length * sizeof(T), data);
    }

    template<typename T>
    void Buffer<T>::AddData(const T *data, uint32_t length)
    {
        if (m_Usage == BufferUsage::Static)
        {
            HC_LOG_WARNING("Buffer::SetData: cannot add data to static buffer");
            return;
        }

        if (m_Capacity <= 1)
            Resize(length);
        else if (m_Size + length > m_Capacity)
        {
            uint32_t newCapacity = m_Capacity * 2;
            while (m_Size + length > newCapacity)
                newCapacity *= 2;
            Resize(newCapacity);
        }
        SetData(data, length, m_Size);
    }

    template<typename T>
    void Buffer<T>::Reserve(uint32_t capacity)
    {
        if (m_Usage == BufferUsage::Static)
        {
            HC_LOG_WARNING("Buffer::SetData: cannot change size of static buffer");
            return;
        }

        if (capacity > m_Capacity)
            Resize(capacity);
    }

    template<typename T>
    void Buffer<T>::Resize(uint32_t newCapacity)
    {
        m_Capacity = newCapacity;

        uint32_t newBufferID = 0;
        glCreateBuffers(1, &newBufferID);
        glNamedBufferStorage(newBufferID, newCapacity * sizeof(T), nullptr, GetGLBufferUsage(m_Usage));

        glCopyNamedBufferSubData(m_HandleID, newBufferID, 0, 0, m_Size * sizeof(T));

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
#pragma once

#include <glad/glad.h>

namespace HrothCore
{
    enum class BufferUsage {
        Static,
        Dynamic
    };

    /* ----- BufferUsage to GL -----*/
    GLenum GetGLBufferUsage(BufferUsage usage);
    /* -----------------------------*/

    template<typename T>
    class Buffer {
        public:
            /* ----- Ctr/Dtr ----- */
            Buffer(BufferUsage usage = BufferUsage::Dynamic)
                : m_Usage(usage), m_Capacity(1)
            {
                glCreateBuffers(1, &m_HandleID);
            }
            
            Buffer(uint32_t capacity, BufferUsage usage = BufferUsage::Dynamic)
                : m_Usage(usage), m_Capacity(capacity)
            {
                glCreateBuffers(1, &m_HandleID);
                glNamedBufferStorage(m_HandleID, m_Capacity * sizeof(T), nullptr, GetGLBufferUsage(m_Usage));
            }

            Buffer(const T *data, uint32_t dataLength, BufferUsage usage = BufferUsage::Dynamic)
                : m_Usage(usage), m_Capacity(dataLength), m_Size(dataLength)
            {
                glCreateBuffers(1, &m_HandleID);
                glNamedBufferStorage(m_HandleID, m_Capacity * sizeof(T), data, GetGLBufferUsage(m_Usage));
            }

            ~Buffer()
            {
                glDeleteBuffers(1, &m_HandleID);
            }
            /* --------------------*/

            /* ----- Data management ----- */
            void SetData(const T *data, uint32_t length, uint32_t offset = 0)
            {
                if (m_Usage == BufferUsage::Static)
                {
                    HC_LOG_WARNING("Buffer::SetData: cannot modify data to static buffer");
                    return;
                }

                HC_LOG_WARNING("Buffer::SetData: data is nullptr");

                if (length + offset <= m_Capacity)
                {
                    uint32_t newCapacity = m_Capacity * 2;
                    while (m_Size + length > newCapacity)
                        newCapacity *= 2;
                    Resize(length + offset);
                }

                if (length + offset > m_Size)
                    m_Size = length + offset;
                glNamedBufferSubData(m_HandleID, offset * sizeof(T), length * sizeof(T), data);
            }

            void AddData(const T *data, uint32_t length)
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

            void Reserve(uint32_t capacity)
            {
                if (m_Usage == BufferUsage::Static)
                {
                    HC_LOG_WARNING("Buffer::SetData: cannot change size of static buffer");
                    return;
                }

                if (capacity > m_Capacity)
                    Resize(capacity);
            }
            /* --------------------------- */

            uint32_t GetID() const { return m_HandleID; }
            uint32_t GetSize() const { return m_Size; }
            uint32_t GetCapacity() const { return m_Capacity; }
        private:
            void Resize(uint32_t newCapacity)
            {
                m_Capacity = newCapacity;

                uint32_t newBufferID = 0;
                glCreateBuffers(1, &newBufferID);
                glNamedBufferStorage(newBufferID, newCapacity * sizeof(T), nullptr, GetGLBufferUsage(m_Usage));

                glCopyNamedBufferSubData(m_HandleID, newBufferID, 0, 0, m_Size * sizeof(T));

                glDeleteBuffers(1, &m_HandleID);
                m_HandleID = newBufferID;
            }

            BufferUsage m_Usage;
            uint32_t m_HandleID = 0;

            uint32_t m_Capacity;
            uint32_t m_Size = 0;
    };
}


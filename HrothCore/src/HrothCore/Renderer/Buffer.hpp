#pragma once

#include <glad/glad.h>

namespace HrothCore
{
    enum class BufferUsage {
        Static,
        Dynamic
    };

    enum class BufferShaderType {
        Uniform,
        ShaderStorage
    };

    /* ----- Buffer Struct to GL -----*/
    GLenum GetGLBufferUsage(BufferUsage usage);
    GLenum GetGLBufferShaderType(BufferShaderType usage);
    /* -----------------------------*/

    template<typename T>
    class Buffer {
        public:
            /* ----- Ctr/Dtr ----- */

            Buffer(BufferUsage usage = BufferUsage::Dynamic)
                : Buffer(1, usage)
            {
            }
            
            Buffer(uint32_t capacity, BufferUsage usage = BufferUsage::Dynamic)
                : m_Usage(usage), m_Capacity(capacity), m_Size(0)
            {
                glCreateBuffers(1, &m_HandleID);
                glNamedBufferStorage(m_HandleID, m_Capacity * sizeof(T), nullptr, GetGLBufferUsage(m_Usage));
            }

            Buffer(const T *data, uint32_t dataLength, BufferUsage usage = BufferUsage::Dynamic)
                : m_Usage(usage), m_Capacity(dataLength), m_Size(dataLength)
            {
                glCreateBuffers(1, &m_HandleID);
                glNamedBufferStorage(m_HandleID, m_Capacity * sizeof(T), nullptr, GetGLBufferUsage(m_Usage));
                SetData(dataLength, data);
            }

            ~Buffer()
            {
            }

            /* --------------------*/

            /* ----- Data management ----- */

            void SetData(uint32_t count, const T *data, uint32_t offset = 0)
            {
                if (m_Usage == BufferUsage::Static)
                {
                    HC_LOG_WARNING("Buffer::SetData: cannot modify data to static buffer");
                    return;
                }
                if (count == 0)
                {
                    HC_LOG_WARNING("Buffer::SetData: count is 0");
                    return;
                }
                if (data == nullptr)
                    HC_LOG_WARNING("Buffer::SetData: data is nullptr");

                if (count + offset > m_Capacity)
                {
                    uint32_t newCapacity = m_Capacity;
                    while (m_Size + count > newCapacity)
                        newCapacity *= 2;
                    Resize(count + offset);
                }

                if (count + offset > m_Size)
                    m_Size = count + offset;
                glNamedBufferSubData(m_HandleID, offset * sizeof(T), count * sizeof(T), data);
            }

            void AddData(uint32_t count, const T *data)
            {
                if (m_Usage == BufferUsage::Static)
                {
                    HC_LOG_WARNING("Buffer::AddData: cannot add data to static buffer");
                    return;
                }

                if (m_Size + count > m_Capacity)
                {
                    uint32_t newCapacity = m_Capacity;
                    while (m_Size + count > newCapacity)
                        newCapacity *= 2;
                    Resize(newCapacity);
                }
                SetData(count, data, m_Size);
            }

            void Reserve(uint32_t capacity)
            {
                if (m_Usage == BufferUsage::Static)
                {
                    HC_LOG_WARNING("Buffer::Reserve: cannot change size of static buffer");
                    return;
                }

                if (capacity > m_Capacity)
                    Resize(capacity);
            }

            void Release()
            {
                glDeleteBuffers(1, &m_HandleID);
            }

            /* --------------------------- */

            /* ----- Binding ----- */

            void BindToShader(uint32_t binding, BufferShaderType type)
            {
                glBindBufferBase(GetGLBufferShaderType(type), binding, m_HandleID);
            }

            void BindIndirectDraw()
            {
                glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_HandleID);
            }

            /* ------------------- */

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


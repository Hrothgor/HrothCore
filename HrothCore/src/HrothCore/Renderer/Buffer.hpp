#pragma once

namespace HrothCore
{
    enum class BufferUsage {
        Static,
        Dynamic
    };

    class Buffer {
        public:
            Buffer(BufferUsage usage = BufferUsage::Dynamic);
            Buffer(uint32_t capacity, BufferUsage usage = BufferUsage::Dynamic);
            Buffer(const void *data, uint32_t dataSize, BufferUsage usage = BufferUsage::Dynamic);
            ~Buffer();

            // must fit in capacity
            void SetData(const void *data, uint32_t size, uint32_t offset = 0);
            // Push_back equivalent
            void AddData(const void *data, uint32_t size);
            void Reserve(uint32_t capacity);

            uint32_t GetID() const { return m_HandleID; }
            uint32_t GetSize() const { return m_Size; }
            uint32_t GetCapacity() const { return m_Capacity; }
        private:
            void Resize(uint32_t newCapacity);

            BufferUsage m_Usage;
            uint32_t m_HandleID = 0;

            uint32_t m_Capacity;
            uint32_t m_Size = 0;
    };
}


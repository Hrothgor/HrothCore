#pragma once

namespace HrothCore
{
    enum class BufferUsage {
        Static,
        Dynamic
    };

    template<typename T>
    class Buffer {
        public:
            Buffer(BufferUsage usage = BufferUsage::Dynamic);
            Buffer(uint32_t capacity, BufferUsage usage = BufferUsage::Dynamic);
            Buffer(const T *data, uint32_t dataLength, BufferUsage usage = BufferUsage::Dynamic);
            ~Buffer();

            // must fit in capacity
            void SetData(const T *data, uint32_t length, uint32_t offset = 0);
            // Push_back equivalent
            void AddData(const T *data, uint32_t length);
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


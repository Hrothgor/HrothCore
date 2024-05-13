#pragma once

namespace HrothCore
{
    enum class BufferUsage {
        Static,
        Dynamic
    };

    class Buffer {
        public:
            Buffer(uint32_t size, const void *data = nullptr, BufferUsage usage = BufferUsage::Dynamic);
            ~Buffer();

            void SetData(uint32_t size, const void *data, uint32_t offset = 0);

            uint32_t GetID() const { return m_HandleID; }
        private:
            BufferUsage m_Usage;
            uint32_t m_Size;
            uint32_t m_HandleID;
    };
}


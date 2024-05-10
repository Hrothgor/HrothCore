#pragma once

namespace HrothCore
{
    enum BufferUsage {
        Static,
        Dynamic,
        Stream
    };

    class Buffer {
        public:
            Buffer();
            ~Buffer();

            void SetData(const void* data, uint32_t size, BufferUsage usage = BufferUsage::Static);

        private:
            uint32_t m_Size;
            uint32_t m_HandleID;
    };
}


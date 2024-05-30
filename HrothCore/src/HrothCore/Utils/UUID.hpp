#pragma once

namespace HrothCore
{
    class UUID
    {
        struct BinaryValue
        {
            uint32_t data1;
            uint16_t data2;
            uint16_t data3;
            uint16_t data4[4];
        };

        public:
            UUID();
            UUID(uint64_t start, uint64_t end);
            UUID(const BinaryValue& uuid);
            UUID(uint32_t data1, uint16_t data2, uint16_t data3, uint16_t data40, uint16_t data41, uint16_t data42, uint16_t data43);

            std::string ToStr() const;
            uint64_t GetStartValue() const;
            uint64_t GetEndValue() const;

            static UUID Generate();
            static UUID FromStr(const std::string &str);

            bool operator==(const UUID& other) const
            {
                return GetStartValue() == other.GetStartValue() && GetEndValue() == other.GetEndValue();
            }

            bool operator!=(const UUID& other) const
            {
                return !operator==(other);
            }

        private:
            BinaryValue m_UuidValue;
    };
}
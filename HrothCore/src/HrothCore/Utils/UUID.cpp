#include "HrothCore_pch.hpp"

#include "HrothCore/Utils/UUID.hpp"

#include <stdio.h>
#include <random>

namespace HrothCore
{
    UUID::UUID()
        : m_UuidValue{}
    {
    }

    UUID::UUID(uint64_t start, uint64_t end)
        : UUID{
            uint32_t((start >> 32) & 0xFFffFFff),
            uint16_t((start >> 16) & 0xFFff),
            uint16_t(start & 0xFFff),
            uint16_t((end >> 48) & 0xFFff),
            uint16_t((end >> 32) & 0xFFff),
            uint16_t((end >> 16) & 0xFFff),
            uint16_t(end & 0xFFff)}
    {
    }

    UUID::UUID(const BinaryValue& uuid)
        : m_UuidValue(uuid)
    {
    }

    UUID::UUID(uint32_t _data1, uint16_t _data2, uint16_t _data3, uint16_t _data40, uint16_t _data41, uint16_t _data42, uint16_t _data43)
        : m_UuidValue{ _data1, _data2, _data3, { _data40, _data41, _data42, _data43 } }
    {
    }

    std::string UUID::ToStr() const
    {
        return std::format("{:08X}-{:04X}-{:04X}-{:04X}-{:04X}{:04X}{:04X}",
            m_UuidValue.data1,
            m_UuidValue.data2,
            m_UuidValue.data3,
            m_UuidValue.data4[0],
            m_UuidValue.data4[1],
            m_UuidValue.data4[2],
            m_UuidValue.data4[3]);
    }

    uint64_t UUID::GetStartValue() const
    {
        return (uint64_t(m_UuidValue.data1) << 32) | (uint64_t(m_UuidValue.data2) << 16) | uint64_t(m_UuidValue.data3);
    }

    uint64_t UUID::GetEndValue() const
    {
        return (uint64_t(m_UuidValue.data4[0]) << 48) | (uint64_t(m_UuidValue.data4[1]) << 32) | (uint64_t(m_UuidValue.data4[2]) << 16) | uint64_t(m_UuidValue.data4[3]);
    }

    static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

    UUID UUID::Generate()
    {
        // random uint64_t
        uint64_t start = s_UniformDistribution(s_Engine);
        uint64_t end = s_UniformDistribution(s_Engine);

        return {
            uint32_t((start >> 32) & 0xFFffFFff),
            uint16_t((start >> 16) & 0xFFff),
            uint16_t((start & 0x0Fff) | 0x4000), // UUID v4 (random)
            uint16_t(((end >> 48) & 0x3Fff) | 0x8000), // Variant
            uint16_t((end >> 32) & 0xFFff),
            uint16_t((end >> 16) & 0xFFff),
            uint16_t(end & 0xFFff)
        };
    }

    UUID UUID::FromStr(const std::string &str)
    {
        uint32_t data1, data2, data3, data40, data41, data42, data43;

#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES // Required for sscanf on window to use sscanf_s instead
        int succes = sscanf(str.c_str(), "%08X-%04X-%04X-%04X-%04X%04X%04X",
            &data1, &data2, &data3, &data40, &data41, &data42, &data43);
#undef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES

        HC_ASSERT(succes == 7, "Failed to parse UUID string!");

        return { data1, (uint16_t)data2, (uint16_t)data3, (uint16_t)data40, (uint16_t)data41, (uint16_t)data42, (uint16_t)data43 };
    }
}
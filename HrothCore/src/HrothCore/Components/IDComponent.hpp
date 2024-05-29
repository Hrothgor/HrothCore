#pragma once

#include <uuid_v4.h>

namespace HrothCore
{
    struct IDComponent
    {
        UUIDv4::UUID UUID;
        std::string Name = "";

        IDComponent() = default;
        IDComponent(const std::string &name);
        IDComponent(const std::string &UUIDstr, const std::string &name);
        ~IDComponent() = default;

        std::string UUIDAsStr();

        bool operator==(const IDComponent &other) const
        {
            return UUID == other.UUID;
        }

        bool operator!=(const IDComponent &other) const
        {
            return !operator==(other);
        }
    };
}
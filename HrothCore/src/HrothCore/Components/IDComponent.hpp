#pragma once

#include "HrothCore/Utils/UUID.hpp"

namespace HrothCore
{
    struct IDComponent
    {
        UUID Uuid;
        std::string Name = "";

        IDComponent() = default;
        IDComponent(const std::string &name);
        IDComponent(const std::string &UUIDstr, const std::string &name);
        ~IDComponent() = default;

        std::string UUIDAsStr();

        bool operator==(const IDComponent &other) const
        {
            return Uuid == other.Uuid;
        }

        bool operator!=(const IDComponent &other) const
        {
            return !operator==(other);
        }
    };
}
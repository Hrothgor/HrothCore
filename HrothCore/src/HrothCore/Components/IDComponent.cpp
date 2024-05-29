#include "HrothCore_pch.hpp"

#include "HrothCore/Components/IDComponent.hpp"

namespace HrothCore
{
    static UUIDv4::UUIDGenerator<std::mt19937_64> uuidGenerator;

    IDComponent::IDComponent(const std::string &name)
        : Name(name)
    {
        UUID = uuidGenerator.getUUID();
    }

    IDComponent::IDComponent(const std::string &UUIDstr, const std::string &name)
        : Name(name)
    {
        UUID = UUIDv4::UUID::fromStrFactory(UUIDstr);
    }

    std::string IDComponent::UUIDAsStr()
    {
        return UUID.str();
    }
}
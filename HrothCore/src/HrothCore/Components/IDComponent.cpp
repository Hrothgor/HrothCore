#include "HrothCore_pch.hpp"

#include "HrothCore/Components/IDComponent.hpp"

namespace HrothCore
{
    IDComponent::IDComponent(const std::string &name)
        : Name(name)
    {
        Uuid = UUID::Generate();
    }

    IDComponent::IDComponent(const std::string &UUIDstr, const std::string &name)
        : Name(name)
    {
        Uuid = UUID::FromStr(UUIDstr);
    }

    std::string IDComponent::UUIDAsStr()
    {
        return Uuid.ToStr();
    }
}
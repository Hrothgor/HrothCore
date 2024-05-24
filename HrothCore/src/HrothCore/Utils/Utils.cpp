#include "HrothCore_pch.hpp"

#include "HrothCore/Utils/Utils.hpp"

namespace HrothCore
{
    bool EndsWith(const char *str, const char *suffix)
    {
        const size_t strLength = strlen(str);
        const size_t suffixLength = strlen(suffix);
        if (strLength < suffixLength)
            return false;
        return strcmp(str + strLength - suffixLength, suffix) == 0;
    }
}
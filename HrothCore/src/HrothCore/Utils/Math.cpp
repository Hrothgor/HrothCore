#include "HrothCore_pch.hpp"

#include "HrothCore/Utils/Math.hpp"

namespace HrothCore
{
    glm::mat4 Math::CreateProjMatrix(float fov, float aspect, float near, float far)
    {
        return glm::perspective(fov, aspect, near, far);
    }
}
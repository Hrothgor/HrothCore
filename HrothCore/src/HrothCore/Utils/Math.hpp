#pragma once

namespace HrothCore
{
    namespace Math
    {
        glm::mat4 CreateProjMatrix(float fov, float aspect, float near, float far);
    };
}
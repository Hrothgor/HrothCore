#pragma once

namespace HrothCore
{
    class Math
    {
        public:
            static glm::mat4 CreateProjMatrix(float fov, float aspect, float near, float far);
    };
}
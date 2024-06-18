#pragma once

namespace HrothCore
{
    enum LightType
    {
        Directional = 0,
        Point,
        Spot
    };

    struct LightComponent
    {
        LightType Type = LightType::Directional;

        glm::vec4 Color = glm::vec4(1.0f);
        float Intensity = 1.0f;
        float Range = 10.0f;
        float FallOff = 1.0f;
        float SpotAngle = 30.0f;

        bool CastsShadows = false;
    };
}

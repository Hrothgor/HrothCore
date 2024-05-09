#pragma once

#include <glm/glm.hpp>

namespace HrothCore
{
    class RenderCommand
    {
    public:
        static void SetClearColor(const glm::vec4 &color);
        static void Clear();

        static void DrawItem();
        static void MultiDrawIndirect();
        static void MultiDrawIndirectSkinned();
    };
}
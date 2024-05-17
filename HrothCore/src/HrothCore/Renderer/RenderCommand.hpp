#pragma once

namespace HrothCore
{
    class RenderCommand
    {
    public:
        static void SetClearColor(const glm::vec4 &color);
        static void Clear();

        static void SetViewport(const glm::ivec2 &size);

        static void DrawItem();
        static void MultiDrawIndirect();
        static void MultiDrawIndirectSkinned();
    };
}
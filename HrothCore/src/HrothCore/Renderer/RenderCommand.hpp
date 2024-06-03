#pragma once

namespace HrothCore
{
    namespace RenderCommand
    {
        void EnableDepthTest(bool enable);
        void SetClearColor(const glm::vec4 &color);
        void Clear();

        void SetViewport(const glm::ivec2 &size);

        void DrawItem();
        void MultiDrawIndirect();
        void MultiDrawIndirectSkinned();
    };
}
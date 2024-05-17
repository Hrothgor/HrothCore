#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/RenderCommand.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    void RenderCommand::SetClearColor(const glm::vec4 &color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RenderCommand::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderCommand::SetViewport(const glm::ivec2 &size)
    {
        glViewport(0, 0, size.x, size.y);
    }

    void RenderCommand::DrawItem()
    {
    }

    void RenderCommand::MultiDrawIndirect()
    {
    }

    void RenderCommand::MultiDrawIndirectSkinned()
    {
    }
}
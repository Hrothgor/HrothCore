#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Framebuffer.hpp"
#include "HrothCore/Renderer/Texture.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    Framebuffer::Framebuffer(std::vector<Texture *> textures, Texture *depthTexture)
        : m_HandleID(0)
    {
        glCreateFramebuffers(1, &m_HandleID);

        for (int i = 0; i < textures.size(); i++)
        {
            if (!textures[i])
                continue;
            glNamedFramebufferTexture(m_HandleID, GL_COLOR_ATTACHMENT0 + i, textures[i]->GetID(), 0);
        }

        if (depthTexture)
            glNamedFramebufferTexture(m_HandleID, GL_DEPTH_ATTACHMENT, depthTexture->GetID(), 0);

        if (glCheckNamedFramebufferStatus(m_HandleID, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            HC_ASSERT(!"Framebuffer is not complete!");
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &m_HandleID);
    }

    void Framebuffer::BlitToColor(Framebuffer *other,
        glm::ivec2 srcLowerBound, glm::ivec2 srcHigherBound,
        glm::ivec2 dstLowerBound, glm::ivec2 dstHigherBound,
        BlitFilterMode filterMode)
    {
    }

    void Framebuffer::BlitToDepth(Framebuffer *other,
        glm::ivec2 srcLowerBound, glm::ivec2 srcHigherBound,
        glm::ivec2 dstLowerBound, glm::ivec2 dstHigherBound,
        BlitFilterMode filterMode)
    {
    }

    void Framebuffer::BlitToStencil(Framebuffer *other,
        glm::ivec2 srcLowerBound, glm::ivec2 srcHigherBound,
        glm::ivec2 dstLowerBound, glm::ivec2 dstHigherBound,
        BlitFilterMode filterMode)
    {
    }
}
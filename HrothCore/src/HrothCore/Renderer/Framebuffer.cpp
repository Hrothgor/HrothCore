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

        m_AttachmentsNumber = static_cast<uint32_t>(textures.size());
        for (uint32_t i = 0; i < m_AttachmentsNumber; i++)
        {
            if (!textures[i])
                continue;
            glNamedFramebufferTexture(m_HandleID, GL_COLOR_ATTACHMENT0 + i, textures[i]->GetID(), 0);
        }

        if (depthTexture)
            glNamedFramebufferTexture(m_HandleID, GL_DEPTH_ATTACHMENT, depthTexture->GetID(), 0);

        GLenum *attachments = new GLenum[m_AttachmentsNumber];
        for (uint32_t i = 0; i < m_AttachmentsNumber; i++)
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        glNamedFramebufferDrawBuffers(m_HandleID, m_AttachmentsNumber, attachments);

        if (glCheckNamedFramebufferStatus(m_HandleID, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            HC_ASSERT(!"Framebuffer is not complete!");
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &m_HandleID);
    }

    void Framebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_HandleID);
    }

    void Framebuffer::ClearColor(glm::vec4 color)
    {
        for (uint32_t i = 0; i < m_AttachmentsNumber; i++)
            ClearColorAttachment(i, color);
    }

    void Framebuffer::ClearColorAttachment(uint32_t attachmentIndex, glm::vec4 color)
    {
        glClearNamedFramebufferfv(m_HandleID, GL_COLOR, GL_COLOR_ATTACHMENT0 + attachmentIndex, glm::value_ptr(color));
    }

    void Framebuffer::ClearDepth(float depth)
    {
        glClearNamedFramebufferfv(m_HandleID, GL_DEPTH, 0, &depth);
    }

    void Framebuffer::Clear()
    {
        ClearColor({1.0, 1.0, 1.0, 1.0});
        ClearDepth(1.0);
    }

    void Framebuffer::BlitToColor(Framebuffer *src, glm::ivec2 srcSize, glm::ivec2 dstSize, BlitFilterMode filterMode)
    {
        GLuint drawID = GetID();
        GLuint readID = src ? src->GetID() : 0;
        GLenum filterGL = GL_NEAREST;

        switch (filterMode)
        {
            case BlitFilterMode::Nearest: filterGL = GL_NEAREST;
            case BlitFilterMode::Linear: filterGL = GL_LINEAR;
        }
            
        glBlitNamedFramebuffer(readID, drawID,
                            0, 0, srcSize.x, srcSize.y,
                            0, 0, dstSize.x, dstSize.y,
                            GL_COLOR_BUFFER_BIT, filterGL);
    }

    void Framebuffer::BlitToColorAttachment(uint32_t myAttachmentIndex, uint32_t srcAttachmentIndex, 
                Framebuffer *src, glm::ivec2 srcSize, glm::ivec2 dstSize, BlitFilterMode filterMode)
    {
        GLuint drawID = GetID();
        GLuint readID = src ? src->GetID() : 0;
        GLenum filterGL = GL_NEAREST;

        switch (filterMode)
        {
            case BlitFilterMode::Nearest: filterGL = GL_NEAREST;
            case BlitFilterMode::Linear: filterGL = GL_LINEAR;
        }
            
        glNamedFramebufferReadBuffer(readID, GL_COLOR_ATTACHMENT0 + srcAttachmentIndex);
        glNamedFramebufferDrawBuffer(drawID, GL_COLOR_ATTACHMENT0 + myAttachmentIndex);
        glBlitNamedFramebuffer(readID, drawID,
                            0, 0, srcSize.x, srcSize.y,
                            0, 0, dstSize.x, dstSize.y,
                            GL_COLOR_BUFFER_BIT, filterGL);
    }

    void Framebuffer::BlitToDepth(Framebuffer *src, glm::ivec2 srcSize, glm::ivec2 dstSize, BlitFilterMode filterMode)
    {
        GLuint drawID = GetID();
        GLuint readID = src ? src->GetID() : 0;
        GLenum filterGL = GL_NEAREST;

        switch (filterMode)
        {
            case BlitFilterMode::Nearest: filterGL = GL_NEAREST;
            case BlitFilterMode::Linear: filterGL = GL_LINEAR;
        }
            
        glBlitNamedFramebuffer(readID, drawID,
                            0, 0, srcSize.x, srcSize.y,
                            0, 0, dstSize.x, dstSize.y,
                            GL_DEPTH_BUFFER_BIT, filterGL);
    }
}
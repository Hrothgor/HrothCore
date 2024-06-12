#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Framebuffer.hpp"
#include "HrothCore/Renderer/Texture.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    Framebuffer::Framebuffer(uint32_t width, uint32_t height)
        : m_Textures(), m_DepthTexture()
        , m_Width(width), m_Height(height)
        , m_HandleID(0)
    {
        glCreateFramebuffers(1, &m_HandleID);
    }

    Framebuffer::~Framebuffer()
    {
    }

    void Framebuffer::CreateTextureAttachment(const std::string &name, TextureInfo info)
    {
        uint32_t index = static_cast<uint32_t>(m_Textures.size());
        m_Textures[name] = Texture(m_Width, m_Height, info);

        glNamedFramebufferTexture(m_HandleID, GL_COLOR_ATTACHMENT0 + index, m_Textures[name].GetID(), 0);

        uint32_t attachementCount = static_cast<uint32_t>(m_Textures.size());
        GLenum *attachments = new GLenum[attachementCount];
        for (uint32_t i = 0; i < attachementCount; i++)
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        glNamedFramebufferDrawBuffers(m_HandleID, attachementCount, attachments);
    }

    void Framebuffer::CreateDepthTextureAttachment(TextureInfo info)
    {
        if (m_DepthTexture.GetID() != 0)
        {
            HC_LOG_WARNING("Framebuffer::CreateDepthTextureAttachment: depth texture already exists");
            return;
        }

        m_DepthTexture = Texture(m_Width, m_Height, info);

        glNamedFramebufferTexture(m_HandleID, GL_DEPTH_ATTACHMENT, m_DepthTexture.GetID(), 0);
    }

    Texture *Framebuffer::GetTexture(const std::string &name)
    {
        if (m_Textures.find(name) == m_Textures.end())
        {
            HC_LOG_WARNING("Framebuffer::GetTexture: texture {0} not found", name);
            return nullptr;
        }
        else 
        {
            return &m_Textures[name];
        }
    }

    Texture *Framebuffer::GetDepthTexture()
    {
        if (m_DepthTexture.GetID() == 0)
        {
            HC_LOG_WARNING("Framebuffer::GetDepthTexture: depth texture not found");
            return nullptr;
        }
        else
        {
            return &m_DepthTexture;
        }
    }

    void Framebuffer::Resize(uint32_t width, uint32_t height)
    {
        HC_ASSERT(width > 0 && height > 0, "Framebuffer::Resize: invalid size");

        if (m_Width == width && m_Height == height)
            return;

        m_Width = width;
        m_Height = height;

        int index = 0;
        for (auto &[name, texture] : m_Textures)
        {
            texture.Resize(width, height);
            glNamedFramebufferTexture(m_HandleID, GL_COLOR_ATTACHMENT0 + index, texture.GetID(), 0);
            index++;
        }

        if (m_DepthTexture.GetID() != 0)
        {
            m_DepthTexture.Resize(width, height);
            glNamedFramebufferTexture(m_HandleID, GL_DEPTH_ATTACHMENT, m_DepthTexture.GetID(), 0);
        }
    }

    void Framebuffer::BindForDrawing() const
    {
        if (m_Textures.size() == 0)
        {
            HC_LOG_WARNING("Framebuffer::BindForDrawing: no attachments to draw");
            return;
        }

        const GLenum status = glCheckNamedFramebufferStatus(m_HandleID, GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            HC_LOG_WARNING("Framebuffer::BindForDrawing: framebuffer is not complete, status: {0}", status);
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, m_HandleID);
        glViewport(0, 0, m_Width, m_Height);
    }

    void Framebuffer::ClearColor(glm::vec4 color)
    {
        for (uint32_t i = 0; i < m_Textures.size(); i++)
            ClearColorAttachment(i, color);
    }

    void Framebuffer::ClearColorAttachment(uint32_t attachmentIndex, glm::vec4 color)
    {
        if (attachmentIndex < 0 || attachmentIndex >= m_Textures.size())
        {
            HC_LOG_WARNING("Framebuffer::ClearColorAttachment: attachment index out of range");
            return;
        }

        glClearNamedFramebufferfv(m_HandleID, GL_COLOR, attachmentIndex, glm::value_ptr(color));
    }

    void Framebuffer::ClearDepth(float depth)
    {
        glClearNamedFramebufferfv(m_HandleID, GL_DEPTH, 0, &depth);
    }

    void Framebuffer::Clear()
    {
        ClearColor();
        ClearDepth();
    }

    void Framebuffer::BlitToColor(Framebuffer *dst, glm::ivec2 dstSize, BlitFilterMode filterMode)
    {
        GLuint readID = GetID();
        GLuint drawID = dst ? dst->GetID() : 0;
        GLenum filterGL = GL_NEAREST;

        switch (filterMode)
        {
            case BlitFilterMode::Nearest: filterGL = GL_NEAREST;
            case BlitFilterMode::Linear: filterGL = GL_LINEAR;
        }
            
        glBlitNamedFramebuffer(readID, drawID,
                            0, 0, m_Width, m_Height,
                            0, 0, dstSize.x, dstSize.y,
                            GL_COLOR_BUFFER_BIT, filterGL);
    }

    void Framebuffer::BlitToColorAttachment(uint32_t myAttachmentIndex, uint32_t dstAttachmentIndex, 
                Framebuffer *dst, glm::ivec2 dstSize, BlitFilterMode filterMode)
    {
        GLuint readID = GetID();
        GLuint drawID = dst ? dst->GetID() : 0;
        GLenum filterGL = GL_NEAREST;

        switch (filterMode)
        {
            case BlitFilterMode::Nearest: filterGL = GL_NEAREST;
            case BlitFilterMode::Linear: filterGL = GL_LINEAR;
        }
            
        glNamedFramebufferDrawBuffer(readID, GL_COLOR_ATTACHMENT0 + myAttachmentIndex);
        glNamedFramebufferReadBuffer(drawID, GL_COLOR_ATTACHMENT0 + dstAttachmentIndex);
        glBlitNamedFramebuffer(readID, drawID,
                            0, 0, m_Width, m_Height,
                            0, 0, dstSize.x, dstSize.y,
                            GL_COLOR_BUFFER_BIT, filterGL);
    }

    void Framebuffer::BlitToDepth(Framebuffer *dst, glm::ivec2 dstSize, BlitFilterMode filterMode)
    {
        GLuint readID = GetID();
        GLuint drawID = dst ? dst->GetID() : 0;
        GLenum filterGL = GL_NEAREST;

        switch (filterMode)
        {
            case BlitFilterMode::Nearest: filterGL = GL_NEAREST;
            case BlitFilterMode::Linear: filterGL = GL_LINEAR;
        }
            
        glBlitNamedFramebuffer(readID, drawID,
                            0, 0, m_Width, m_Height,
                            0, 0, dstSize.x, dstSize.y,
                            GL_DEPTH_BUFFER_BIT, filterGL);
    }

    void Framebuffer::Release()
    {
        glDeleteFramebuffers(1, &m_HandleID);
        for (auto &texture : m_Textures)
            texture.second.Release();
        m_DepthTexture.Release();
    }
}
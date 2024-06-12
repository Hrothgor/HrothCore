#pragma once

#include "HrothCore/Renderer/Texture.hpp"

namespace HrothCore
{
    enum class BlitFilterMode
    {
        Nearest,
        Linear
    };

    class Framebuffer
    {
        public:
            Framebuffer(uint32_t width, uint32_t height);
            ~Framebuffer();

            void CreateTextureAttachment(const std::string &name, TextureInfo info = TextureInfo());
            void CreateDepthTextureAttachment(TextureInfo info = TextureInfo());
            Texture *GetTexture(const std::string &name);
            Texture *GetDepthTexture();

            void Resize(uint32_t width, uint32_t height);
            uint32_t GetWidth() const { return m_Width; }
            uint32_t GetHeight() const { return m_Height; }

            void BindForDrawing() const;

            void ClearColor(glm::vec4 color = glm::vec4(0.0, 0.0, 0.0, 1.0));
            void ClearColorAttachment(uint32_t attachmentIndex, glm::vec4 color = glm::vec4(0.0, 0.0, 0.0, 1.0));
            void ClearDepth(float depth = 0.0);
            void Clear();

            void BlitToColor(Framebuffer *dst, glm::ivec2 dstSize, BlitFilterMode filterMode);
            void BlitToColorAttachment(uint32_t myAttachmentIndex, uint32_t dstAttachmentIndex, 
                Framebuffer *dst, glm::ivec2 dstSize, BlitFilterMode filterMode);
            void BlitToDepth(Framebuffer *dst, glm::ivec2 dstSize, BlitFilterMode filterMode); 

            uint32_t GetID() const { return m_HandleID; }

            void Release();
        private:
            std::unordered_map<std::string, Texture> m_Textures;
            Texture m_DepthTexture;

            uint32_t m_Width, m_Height;

            uint32_t m_HandleID;
    };
}
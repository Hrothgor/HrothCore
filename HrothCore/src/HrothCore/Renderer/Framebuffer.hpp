#pragma once

namespace HrothCore
{
    class Texture;

    class Framebuffer
    {
        enum class BlitFilterMode
        {
            Nearest,
            Linear
        };

        public:
            Framebuffer(std::vector<Texture *> textures, Texture *depthTexture = nullptr);
            ~Framebuffer();

            void Bind() const;

            void ClearColor(glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0));
            void ClearColorAttachment(uint32_t attachmentIndex, glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0));
            void ClearDepth(float depth = 1.0);
            void Clear();

            void BlitToColor(Framebuffer *src, glm::ivec2 srcSize, glm::ivec2 dstSize, BlitFilterMode filterMode);
            void BlitToColorAttachment(uint32_t myAttachmentIndex, uint32_t srcAttachmentIndex, 
                Framebuffer *src, glm::ivec2 srcSize, glm::ivec2 dstSize, BlitFilterMode filterMode);
            void BlitToDepth(Framebuffer *src, glm::ivec2 srcSize, glm::ivec2 dstSize, BlitFilterMode filterMode); 

            uint32_t GetID() const { return m_HandleID; }
        private:
            uint32_t m_AttachmentsNumber;
            uint32_t m_HandleID;
    };
}
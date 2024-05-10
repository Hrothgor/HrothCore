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

            void BlitToColor(Framebuffer *other,
                glm::ivec2 srcLowerBound, glm::ivec2 srcHigherBound,
                glm::ivec2 dstLowerBound, glm::ivec2 dstHigherBound,
                BlitFilterMode filterMode);
            void BlitToDepth(Framebuffer *other,
                glm::ivec2 srcLowerBound, glm::ivec2 srcHigherBound,
                glm::ivec2 dstLowerBound, glm::ivec2 dstHigherBound,
                BlitFilterMode filterMode);
            void BlitToStencil(Framebuffer *other,
                glm::ivec2 srcLowerBound, glm::ivec2 srcHigherBound,
                glm::ivec2 dstLowerBound, glm::ivec2 dstHigherBound,
                BlitFilterMode filterMode);
        private:
            uint32_t m_AttachmentsNumber;
            uint32_t m_HandleID;
    };
}
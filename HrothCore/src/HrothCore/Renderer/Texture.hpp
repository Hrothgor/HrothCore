#pragma once

namespace HrothCore
{
    struct TextureInfo
    {
        enum class WrapMode
        {
            Repeat, MirroredRepeat,
            ClampToEdge, ClampToBorder
        } wrapMode = WrapMode::Repeat;

        enum class FilterMode
        {
            Nearest,
            Linear
        } filterMode = FilterMode::Linear;

        enum class Format
        {
            R, RG, RGB,
            RGBA
        } format = Format::RGBA;

        enum class DataType
        {
            UByte, Byte,
            UShort, Short,
            UInt, Int,
            Float
        } dataType = DataType::UByte;

        bool mipmapped = false;
    };

    class Texture
    {
        public:
            Texture();
            Texture(uint32_t width, uint32_t height, TextureInfo info = TextureInfo());
            ~Texture();

            void SetData(const void *data);
            void BindTextureUnit(uint32_t unit = 0) const;

            uint32_t GetWidth() const { return m_Width; }
            uint32_t GetHeight() const { return m_Height; }
            void Resize(uint32_t width, uint32_t height);

            uint32_t GetID() const { return m_HandleID; }
            uint64_t GetBindlessID() const { return m_BindlessID; }
        private:
            void CreateTexture();

            TextureInfo m_Info;
            uint32_t m_Width, m_Height;
            uint32_t m_HandleID;
            uint64_t m_BindlessID;
    };
}
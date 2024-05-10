#pragma once

namespace HrothCore
{
    struct TextureInfo
    {
        enum class WrapMode
        {
            Repeat, MirroredRepeat,
            ClampToEdge, ClampToBorder
        } wrapMode;

        enum class FilterMode
        {
            Nearest,
            Linear
        } filterMode;

        enum class Format
        {
            R, RG, RGB,
            RGBA
        } format;

        enum class DataType
        {
            UByte, Byte,
            UShort, Short,
            UInt, Int,
            Float
        } dataType;

        bool mipmapped;

        TextureInfo()
            : wrapMode(WrapMode::Repeat), filterMode(FilterMode::Linear)
            , format(Format::RGBA), dataType(DataType::UByte)
            , mipmapped(false)
        {
        }
    };

    class Texture
    {
        public:
            Texture(uint32_t width, uint32_t height, TextureInfo info = TextureInfo());
            ~Texture();

            void SetData(const void *data);
            void BindTextureUnit(uint32_t unit = 0) const;

            void Resize(uint32_t width, uint32_t height);

            uint32_t GetID() const { return m_HandleID; }
        private:
            void CreateTexture();

            TextureInfo m_Info;
            uint32_t m_Width, m_Height;
            uint32_t m_HandleID;
            uint32_t m_BindlessID;
    };
}
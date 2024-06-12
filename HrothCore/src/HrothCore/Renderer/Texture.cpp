#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Texture.hpp"

#include <glad/glad.h>

namespace HrothCore
{
    /* ----- TextureInfo to GL -----*/
    GLint GetGLWrapMode(TextureInfo::WrapMode mode);
    GLint GetGLFilterMode(TextureInfo::FilterMode mode);
    GLenum GetGLFormat(TextureInfo::Format format);
    GLenum GetGLDataType(TextureInfo::DataType type);
    GLenum ComputeGLInternalFormat(TextureInfo::Format format, TextureInfo::DataType dataType);
    /* -----------------------------*/

    Texture::Texture()
        : m_Info(), m_Width(0), m_Height(0), m_HandleID(0)
    {
    }

    Texture::Texture(uint32_t width, uint32_t height, TextureInfo info)
        : m_Info(info), m_Width(width), m_Height(height), m_HandleID(0)
    {
        CreateTexture();
    }

    Texture::~Texture()
    {
    }

    void Texture::CreateTexture()
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_HandleID);
        if (m_Info.mipmapped)
            glGenerateTextureMipmap(m_HandleID);
        glTextureParameteri(m_HandleID, GL_TEXTURE_WRAP_S, GetGLWrapMode(m_Info.wrapMode));
        glTextureParameteri(m_HandleID, GL_TEXTURE_WRAP_T, GetGLWrapMode(m_Info.wrapMode));
        glTextureParameteri(m_HandleID, GL_TEXTURE_MAG_FILTER, GetGLFilterMode(m_Info.filterMode));
        glTextureParameteri(m_HandleID, GL_TEXTURE_MIN_FILTER, GetGLFilterMode(m_Info.filterMode));

        glTextureStorage2D(m_HandleID, 1, ComputeGLInternalFormat(m_Info.format, m_Info.dataType), m_Width, m_Height);
        
        if (m_Info.bindless)
        {
            m_BindlessID = glGetTextureHandleARB(m_HandleID);
            glMakeTextureHandleResidentARB(m_BindlessID);
        }
    }

    void Texture::SetData(const void *data)
    {
        glTextureSubImage2D(m_HandleID, 0, 0, 0,
                        m_Width, m_Height,
                        GetGLFormat(m_Info.format), GetGLDataType(m_Info.dataType),
                        data);
    }

    void Texture::BindTextureUnit(uint32_t unit) const
    {
        glBindTextureUnit(unit, m_HandleID);
    }

    void Texture::Resize(uint32_t width, uint32_t height)
    {
        HC_ASSERT(width > 0 && height > 0, "Texture::Resize: width and height must be greater than 0");
        
        if (m_Width == width && m_Height == height)
            return;

        Release();
        m_Width = width;
        m_Height = height;
        CreateTexture();
    }

    uint64_t Texture::GetBindlessID() const
    {
        if (!m_Info.bindless)
        {
            HC_LOG_WARNING("Texture::GetBindlessID: texture is not bindless");
            return 0;
        }

        return m_BindlessID;
    }

    void Texture::Release()
    {
        if (m_Info.bindless)
            glMakeTextureHandleNonResidentARB(m_BindlessID);
        glDeleteTextures(1, &m_HandleID);
    }

    /* ----- TextureInfo to GL -----*/

    GLint GetGLWrapMode(TextureInfo::WrapMode mode)
    {
        switch (mode)
        {
            case TextureInfo::WrapMode::Repeat: return GL_REPEAT;
            case TextureInfo::WrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
            case TextureInfo::WrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
            case TextureInfo::WrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
        }
        return 0;
    }

    GLint GetGLFilterMode(TextureInfo::FilterMode mode)
    {
        switch (mode)
        {
            case TextureInfo::FilterMode::Nearest: return GL_NEAREST;
            case TextureInfo::FilterMode::Linear: return GL_LINEAR;
        }
        return 0;
    }

    GLenum GetGLFormat(TextureInfo::Format format)
    {
        switch (format)
        {
            case TextureInfo::Format::R: return GL_RED;
            case TextureInfo::Format::RG: return GL_RG;
            case TextureInfo::Format::RGB: return GL_RGB;
            case TextureInfo::Format::RGBA: return GL_RGBA;
        }
        return 0;
    }

    GLenum GetGLDataType(TextureInfo::DataType type)
    {
        switch (type)
        {
            case TextureInfo::DataType::UByte: return GL_UNSIGNED_BYTE;
            case TextureInfo::DataType::Byte: return GL_BYTE;
            case TextureInfo::DataType::UShort: return GL_UNSIGNED_SHORT;
            case TextureInfo::DataType::Short: return GL_SHORT;
            case TextureInfo::DataType::UInt: return GL_UNSIGNED_INT;
            case TextureInfo::DataType::Int: return GL_INT;
            case TextureInfo::DataType::Float: return GL_FLOAT;
        }
        return 0;
    }

    GLenum ComputeGLInternalFormat(TextureInfo::Format format, TextureInfo::DataType dataType)
    {
        switch (format)
        {
            case TextureInfo::Format::R:
                switch (dataType)
                {
                    case TextureInfo::DataType::UByte: return GL_R8;
                    case TextureInfo::DataType::Byte: return GL_R8_SNORM;
                    case TextureInfo::DataType::UShort: return GL_R16;
                    case TextureInfo::DataType::Short: return GL_R16_SNORM;
                    case TextureInfo::DataType::UInt: return GL_R32UI;
                    case TextureInfo::DataType::Int: return GL_R32I;
                    case TextureInfo::DataType::Float: return GL_R32F;
                }
            case TextureInfo::Format::RG:
                switch (dataType)
                {
                    case TextureInfo::DataType::UByte: return GL_RG8;
                    case TextureInfo::DataType::Byte: return GL_RG8_SNORM;
                    case TextureInfo::DataType::UShort: return GL_RG16;
                    case TextureInfo::DataType::Short: return GL_RG16_SNORM;
                    case TextureInfo::DataType::UInt: return GL_RG32UI;
                    case TextureInfo::DataType::Int: return GL_RG32I;
                    case TextureInfo::DataType::Float: return GL_RG32F;
                }
            case TextureInfo::Format::RGB:
                switch (dataType)
                {
                    case TextureInfo::DataType::UByte: return GL_RGB8;
                    case TextureInfo::DataType::Byte: return GL_RGB8_SNORM;
                    case TextureInfo::DataType::UShort: return GL_RGB16;
                    case TextureInfo::DataType::Short: return GL_RGB16_SNORM;
                    case TextureInfo::DataType::UInt: return GL_RGB32UI;
                    case TextureInfo::DataType::Int: return GL_RGB32I;
                    case TextureInfo::DataType::Float: return GL_RGB32F;
                }
            case TextureInfo::Format::RGBA:
                switch (dataType)
                {
                    case TextureInfo::DataType::UByte: return GL_RGBA8;
                    case TextureInfo::DataType::Byte: return GL_RGBA8_SNORM;
                    case TextureInfo::DataType::UShort: return GL_RGBA16;
                    case TextureInfo::DataType::Short: return GL_RGBA16_SNORM;
                    case TextureInfo::DataType::UInt: return GL_RGBA32UI;
                    case TextureInfo::DataType::Int: return GL_RGBA32I;
                    case TextureInfo::DataType::Float: return GL_RGBA32F;
                }
        }
        return 0;
    }

    /* -----------------------------*/
}
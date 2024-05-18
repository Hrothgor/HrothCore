#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Buffer.hpp"

namespace HrothCore
{
    /* ----- Buffer Struct to GL -----*/
    GLenum GetGLBufferUsage(BufferUsage usage)
    {
        switch (usage)
        {
            case BufferUsage::Static: return 0;
            case BufferUsage::Dynamic: return GL_DYNAMIC_STORAGE_BIT;
        }
        return 0;
    }

    GLenum GetGLBufferShaderType(BufferShaderType type)
    {
        switch (type)
        {
            case BufferShaderType::Uniform: return GL_UNIFORM_BUFFER;
            case BufferShaderType::ShaderStorage: return GL_SHADER_STORAGE_BUFFER;
        }
        return 0;
    }
    /* -----------------------------*/
}


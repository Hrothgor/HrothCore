#include "HrothCore_pch.hpp"

#include "HrothCore/Renderer/Buffer.hpp"

namespace HrothCore
{
    /* ----- BufferUsage to GL -----*/
    GLenum GetGLBufferUsage(BufferUsage usage)
    {
        switch (usage)
        {
            case BufferUsage::Static: return 0;
            case BufferUsage::Dynamic: return GL_DYNAMIC_STORAGE_BIT;
        }
        return 0;
    }
    /* -----------------------------*/
}


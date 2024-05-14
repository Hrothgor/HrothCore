#pragma once

namespace HrothCore
{
    struct Mesh
    {
        uint32_t VerticesCount;
        uint32_t IndicesCount;

        uint32_t BaseVertex;
        uint32_t BaseIndex;
    };
}
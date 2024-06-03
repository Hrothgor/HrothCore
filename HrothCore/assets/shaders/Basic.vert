#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textCoord;

layout(std140, binding = 0) uniform PerFrameData
{
    uniform mat4 view;
    uniform mat4 proj;
};

struct PerMeshData_t
{
    mat4 model;
};

layout(std430, binding = 0) readonly buffer perMeshData {
    PerMeshData_t PerMeshData[];
};

layout (location = 0) out vec3 color;

void main() {
    mat4 MVP = proj * view * PerMeshData[gl_DrawID].model;
    gl_Position = MVP * vec4(pos, 1.0);

    color = pos;
}
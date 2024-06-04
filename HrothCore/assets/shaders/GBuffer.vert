#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout(std140, binding = 0) uniform PerFrameData
{
    uniform mat4 iView;
    uniform mat4 iProj;
    uniform vec2 iResolution;
};

struct PerMeshData_t
{
    mat4 model;
};

layout(std430, binding = 0) readonly buffer perMeshData {
    PerMeshData_t PerMeshData[];
};

out vec3 color;
out vec3 worldNormal;
out vec3 localNormal;
out vec2 texCoord;

void main() {
    mat4 MVP = iProj * iView * PerMeshData[gl_DrawID].model;
    gl_Position = MVP * vec4(aPos, 1.0);

    color = aPos;
    worldNormal = aNormal;
    localNormal = normalize(PerMeshData[gl_DrawID].model * vec4(aNormal, 0.0)).xyz;
    texCoord = aTexCoord;
}
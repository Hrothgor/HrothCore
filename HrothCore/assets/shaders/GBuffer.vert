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

struct Material
{
    int diffuseTextureIndex;
    int specularTextureIndex;
    int normalTextureIndex;
    int occlusionTextureIndex;
    int emissiveTextureIndex;

    float occlusionStrength;
    float emissiveIntensity;

    vec3 color;

    float shininess;
    float reflectivity;
};

struct PerMeshData_t
{
    mat4 model;
    Material material;
};

layout(std430, binding = 1) readonly buffer perMeshData {
    PerMeshData_t PerMeshData[];
};

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;
// MATERIAL
out flat int DiffuseTextureIndex;
out flat int SpecularTextureIndex;
out flat int NormalTextureIndex;
out flat int OcclusionTextureIndex;
out flat int EmissiveTextureIndex;
out float OcclusionStrength;
out float EmissiveIntensity;
out vec3 Color;
out float Shininess;
out float Reflectivity;

void main() {
    mat4 MVP = iProj * iView * PerMeshData[gl_DrawID].model;
    gl_Position = MVP * vec4(aPos, 1.0);

    FragPos = (PerMeshData[gl_DrawID].model * vec4(aPos, 1.0)).xyz;
    TexCoord = aTexCoord;
    Normal = normalize(PerMeshData[gl_DrawID].model * vec4(aNormal, 0.0)).xyz;

    DiffuseTextureIndex = PerMeshData[gl_DrawID].material.diffuseTextureIndex;
    SpecularTextureIndex = PerMeshData[gl_DrawID].material.specularTextureIndex;
    NormalTextureIndex = PerMeshData[gl_DrawID].material.normalTextureIndex;
    OcclusionTextureIndex = PerMeshData[gl_DrawID].material.occlusionTextureIndex;
    EmissiveTextureIndex = PerMeshData[gl_DrawID].material.emissiveTextureIndex;

    OcclusionStrength = PerMeshData[gl_DrawID].material.occlusionStrength;
    EmissiveIntensity = PerMeshData[gl_DrawID].material.emissiveIntensity;

    Color = PerMeshData[gl_DrawID].material.color;

    Shininess = PerMeshData[gl_DrawID].material.shininess;
    Reflectivity = PerMeshData[gl_DrawID].material.reflectivity;
}
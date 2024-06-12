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

    vec3 diffuseColor;
    vec3 specularColor;
    vec3 ambientColor;
    vec3 emissiveColor;

    float occlusionStrength;

    float shininess;
    float diffuseReflectivity;
    float specularReflectivity;
};

struct PerMeshData_t
{
    mat4 model;
    Material material;
};

layout(std430, binding = 1) readonly buffer perMeshData {
    PerMeshData_t PerMeshData[];
};

out vec2 TexCoord;
out vec3 Normal;
// MATERIAL
out flat int DiffuseTextureIndex;
out flat int SpecularTextureIndex;
out flat int NormalTextureIndex;
out flat int OcclusionTextureIndex;
out flat int EmissiveTextureIndex;
out vec3 DiffuseColor;
out vec3 SpecularColor;
out vec3 AmbientColor;
out vec3 EmissiveColor;
out float OcclusionStrength;
out float Shininess;
out float DiffuseReflectivity;
out float SpecularReflectivity;

void main() {
    mat4 MVP = iProj * iView * PerMeshData[gl_DrawID].model;
    gl_Position = MVP * vec4(aPos, 1.0);

    TexCoord = aTexCoord;
    Normal = normalize(PerMeshData[gl_DrawID].model * vec4(aNormal, 0.0)).xyz;

    DiffuseTextureIndex = PerMeshData[gl_DrawID].material.diffuseTextureIndex;
    SpecularTextureIndex = PerMeshData[gl_DrawID].material.specularTextureIndex;
    NormalTextureIndex = PerMeshData[gl_DrawID].material.normalTextureIndex;
    OcclusionTextureIndex = PerMeshData[gl_DrawID].material.occlusionTextureIndex;
    EmissiveTextureIndex = PerMeshData[gl_DrawID].material.emissiveTextureIndex;

    DiffuseColor = PerMeshData[gl_DrawID].material.diffuseColor;
    SpecularColor = PerMeshData[gl_DrawID].material.specularColor;
    AmbientColor = PerMeshData[gl_DrawID].material.ambientColor;
    EmissiveColor = PerMeshData[gl_DrawID].material.emissiveColor;

    OcclusionStrength = PerMeshData[gl_DrawID].material.occlusionStrength;

    Shininess = PerMeshData[gl_DrawID].material.shininess;
    DiffuseReflectivity = PerMeshData[gl_DrawID].material.diffuseReflectivity;
    SpecularReflectivity = PerMeshData[gl_DrawID].material.specularReflectivity;
}
#version 460 core
#extension GL_ARB_bindless_texture : enable

in vec2 TexCoord;
in vec3 Normal;
// MATERIAL
in flat int DiffuseTextureIndex;
in flat int SpecularTextureIndex;
in flat int NormalTextureIndex;
in flat int OcclusionTextureIndex;
in flat int EmissiveTextureIndex;
in vec3 DiffuseColor;
in vec3 SpecularColor;
in vec3 AmbientColor;
in vec3 EmissiveColor;
in float OcclusionStrength;
in float Shininess;
in float DiffuseReflectivity;
in float SpecularReflectivity;

layout (location = 0) out vec4 gTex1;
layout (location = 1) out vec4 gTex2;
layout (location = 2) out vec4 gTex3;
layout (location = 3) out vec4 gTex4;

layout(std430, binding = 0) readonly restrict buffer textureSamplers { 
	uvec2 TextureSamplers[]; 
};

void main()
{
    if(DiffuseTextureIndex == -1)
        gTex1 = vec4(DiffuseColor, 1.0);
    else
        gTex1 = texture(sampler2D(TextureSamplers[DiffuseTextureIndex]), TexCoord);
    gTex2 = vec4(0.0,1.0,0.0, 1.0);
    gTex3 = vec4(1.0,0.0,0.0, 1.0);
    gTex4 = vec4(0.0,0.0,1.0, 1.0);
}
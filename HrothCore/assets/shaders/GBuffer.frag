#version 460 core
#extension GL_ARB_bindless_texture : enable

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
// MATERIAL
in flat int DiffuseTextureIndex;
in flat int SpecularTextureIndex;
in flat int NormalTextureIndex;
in flat int OcclusionTextureIndex;
in flat int EmissiveTextureIndex;
in float OcclusionStrength;
in float EmissiveIntensity;
in vec3 Color;
in float Shininess;
in float Reflectivity;

layout (location = 0) out vec4 gTex1;
layout (location = 1) out vec4 gTex2;
layout (location = 2) out vec4 gTex3;
layout (location = 3) out vec4 gTex4;

layout(std430, binding = 0) readonly restrict buffer textureSamplers { 
	uvec2 TextureSamplers[]; 
};

void main()
{
    // gTex1 RGB = ALBEDO, A = Specular
    gTex1.rgb = DiffuseTextureIndex == -1 ?
                    Color :
                    texture(sampler2D(TextureSamplers[DiffuseTextureIndex]), TexCoord).rgb;
    gTex1.a = SpecularTextureIndex == -1 ?
                    1.0 :
                    texture(sampler2D(TextureSamplers[SpecularTextureIndex]), TexCoord).r;
    // gTex2 RGB  = NORMAL, A = Shininess
    gTex2.rgb = NormalTextureIndex == -1 ?
                    Normal :
                    normalize(texture(sampler2D(TextureSamplers[NormalTextureIndex]), TexCoord).xyz);
    gTex2.a = Shininess;
    // gTex3 RGB = EMISSIVE, A = Reflectivity
    gTex3.rgb = EmissiveTextureIndex == -1 ?
                    vec3(0.0) :
                    texture(sampler2D(TextureSamplers[EmissiveTextureIndex]), TexCoord).rgb * EmissiveIntensity;
    gTex3.a = Reflectivity;
    // gTex4 RGB = unused, A = OCCLUSION
    // gTex4.rgb = vec3(FragPos.x, FragPos.y, FragPos.z);
    gTex4.a = OcclusionTextureIndex == -1 ?
                    1.0 :
                    texture(sampler2D(TextureSamplers[OcclusionTextureIndex]), TexCoord).r * OcclusionStrength;
}
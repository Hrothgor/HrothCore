#version 460 core

layout (location = 0) in vec3 color;
layout (location = 1) in vec3 worldNormal;
layout (location = 2) in vec3 localNormal;
layout (location = 3) in vec2 texCoord;

layout (location = 0) out vec4 gTex1;
layout (location = 1) out vec4 gTex2;
layout (location = 2) out vec4 gTex3;
layout (location = 3) out vec4 gTex4;

void main()
{
    gTex1 = vec4(color, 1.0);
    gTex2 = vec4(worldNormal, 1.0);
    gTex3 = vec4(localNormal, 1.0);
    gTex4 = vec4(vec3(texCoord, 0.0), 1.0);
}
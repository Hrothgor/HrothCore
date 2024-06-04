#version 460 core

in vec3 color;
in vec3 worldNormal;
in vec3 localNormal;
in vec2 texCoord;

layout (location = 0) out vec4 gTex1;
layout (location = 1) out vec4 gTex2;
layout (location = 2) out vec4 gTex3;
layout (location = 3) out vec4 gTex4;

void main()
{
    gTex1 = vec4(1.0,1.0,1.0, 1.0);
    gTex2 = vec4(0.0,1.0,0.0, 1.0);
    gTex3 = vec4(1.0,0.0,0.0, 1.0);
    gTex4 = vec4(0.0,0.0,1.0, 1.0);
}
#version 460 core

in vec2 uv;

layout(std140, binding = 0) uniform PerFrameData
{
    uniform mat4 iView;
    uniform mat4 iProj;
    uniform vec2 iResolution;
};

layout (binding = 0) uniform sampler2D iTex1;
layout (binding = 1) uniform sampler2D iTex2;
layout (binding = 2) uniform sampler2D iTex3;
layout (binding = 3) uniform sampler2D iTex4;
layout (binding = 4) uniform sampler2D iTexDepth;

layout (location = 0) out vec4 FragColor;

void main()
{
    vec2 fragCoord = uv * iResolution;
    
    // 3*3 grid
    vec2 gridSpacing = iResolution / 3;
    float gridThickness = 2.0;
    // Draw border around textures
    if (mod(fragCoord.x, gridSpacing.x) < gridThickness || mod(fragCoord.y, gridSpacing.y) < gridThickness)
    {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        return;
    }

    vec2 gridSpaceUV = uv * 3.0;

    // gTex1 RGB = ALBEDO A = Specular
    if (fragCoord.x < gridSpacing.x
         && fragCoord.y < gridSpacing.y)
    {
        FragColor = vec4(texture(iTex1, vec2(gridSpaceUV.x, gridSpaceUV.y)).rgb, 1.0);
        return;
    }
    if ((fragCoord.x > gridSpacing.x && fragCoord.x < gridSpacing.x * 2)
         && fragCoord.y < gridSpacing.y)
    {
        FragColor = vec4(vec3(texture(iTex1, vec2(gridSpaceUV.x, gridSpaceUV.y)).a), 1.0);
        return;
    }
    // gTex2 RGB = NORMAL A = Shininess
    if (fragCoord.x > gridSpacing.x * 2
         && fragCoord.y < gridSpacing.y)
    {
        FragColor = vec4(texture(iTex2, vec2(gridSpaceUV.x, gridSpaceUV.y)).rgb, 1.0);
        return;
    }
    if (fragCoord.x < gridSpacing.x
         && (fragCoord.y > gridSpacing.y && fragCoord.y < gridSpacing.y * 2))
    {
        FragColor = vec4(vec3(texture(iTex2, vec2(gridSpaceUV.x, gridSpaceUV.y)).a), 1.0);
        return;
    }
    // gTex3 RGB = EMISSIVE A = Reflectivity
    if ((fragCoord.x > gridSpacing.x && fragCoord.x < gridSpacing.x * 2)
         && (fragCoord.y > gridSpacing.y && fragCoord.y < gridSpacing.y * 2))
    {
        FragColor = vec4(texture(iTex3, vec2(gridSpaceUV.x, gridSpaceUV.y)).rgb, 1.0);
        return;
    }
    if (fragCoord.x > gridSpacing.x * 2
         && (fragCoord.y > gridSpacing.y && fragCoord.y < gridSpacing.y * 2))
    {
        FragColor = vec4(vec3(texture(iTex3, vec2(gridSpaceUV.x, gridSpaceUV.y)).a), 1.0);
        return;
    }
    // gTex4 RGB = POSITION A = OCCLUSION
    if (fragCoord.x < gridSpacing.x
         && fragCoord.y > gridSpacing.y * 2)
    {
        FragColor = vec4(texture(iTex4, vec2(gridSpaceUV.x, gridSpaceUV.y)).rgb, 1.0);
        return;
    }
    if ((fragCoord.x > gridSpacing.x && fragCoord.x < gridSpacing.x * 2)
         && fragCoord.y > gridSpacing.y * 2)
    {
        FragColor = vec4(vec3(texture(iTex4, vec2(gridSpaceUV.x, gridSpaceUV.y)).a), 1.0);
        return;
    }
    // gTexDepth RGB = DEPTH A = unused
    if (fragCoord.x > gridSpacing.x * 2
         && fragCoord.y > gridSpacing.y * 2)
    {
        FragColor = vec4(vec3(texture(iTexDepth, vec2(gridSpaceUV.x, gridSpaceUV.y)).r), 1.0);
        return;
    }
}   
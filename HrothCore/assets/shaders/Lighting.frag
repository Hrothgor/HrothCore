#version 460 core

in vec2 uv;

layout(std140, binding = 0) uniform PerFrameData
{
    uniform mat4 iView;
    uniform mat4 iProj;
    uniform vec2 iResolution;
    uniform int iLightCount;
};

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

struct Light
{
    vec3 position;
    vec3 rotation;
    int type; // 0: Directional, 1: Point, 2: Spot
    vec4 color;
    float intensity;
    float range;
    float fallOff;
    float spotAngle;
};

layout(std430, binding = 2) readonly buffer lights
{
    Light Lights[];
};

layout (binding = 0) uniform sampler2D iTex1;
layout (binding = 1) uniform sampler2D iTex2;
layout (binding = 2) uniform sampler2D iTex3;
layout (binding = 3) uniform sampler2D iTex4;
layout (binding = 4) uniform sampler2D iTexDepth;

layout (location = 0) out vec4 FragColor;

void main()
{
    vec3 Albedo = texture(iTex1, uv).rgb;
    float Specular = texture(iTex1, uv).a;
    vec3 Normal = texture(iTex2, uv).rgb;
    float Shininess = texture(iTex2, uv).a;
    vec3 Emissive = texture(iTex3, uv).rgb;
    float Reflectivity = texture(iTex3, uv).a;
    vec3 Position = texture(iTex4, uv).rgb;
    float Occlusion = texture(iTex4, uv).a;

    vec3 lighting = Albedo * 0.2; // hard-coded ambient component
    for (int i = 0; i < iLightCount; i++)
    {
        Light light = Lights[i];
        vec3 lightDir = normalize(light.position - Position);
        float lightDistance = length(light.position - Position);
        if (lightDistance > light.range)
            continue;
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * light.color.rgb;
        lighting += diffuse;
    }

    FragColor = vec4(lighting, 1.0);
}
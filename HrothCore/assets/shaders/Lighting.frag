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

vec3 WorldPosFromDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // range [0, 1] -> [-1, 1]

    vec4 clipSpacePosition = vec4(uv * 2.0 - 1.0, z, 1.0);
    vec4 viewSpacePosition = inverse(iProj) * clipSpacePosition;

    // Perspective division
    viewSpacePosition /= viewSpacePosition.w;

    vec4 worldSpacePosition = inverse(iView) * viewSpacePosition;

    return worldSpacePosition.xyz;
}

void main()
{
    vec3 Albedo = texture(iTex1, uv).rgb;
    float Specular = texture(iTex1, uv).a;
    vec3 Normal = texture(iTex2, uv).rgb;
    float Shininess = texture(iTex2, uv).a;
    vec3 Emissive = texture(iTex3, uv).rgb;
    float Reflectivity = texture(iTex3, uv).a;
    vec3 Position = WorldPosFromDepth(texture(iTexDepth, uv).r);
    float Occlusion = texture(iTex4, uv).a;

    vec3 viewDir = normalize((inverse(iView) * vec4(0,0,0,1)).xyz - Position);

    vec3 lighting = Albedo * 0.2; // hard-coded ambient component
    for (int i = 0; i < iLightCount; i++)
    {
        Light light = Lights[i];

        vec3 lightPos = light.position;
        vec3 lightColor = light.color.rgb;
        float lightIntensity = light.intensity;
        float lightFallOff = light.fallOff;
        float lightSpotAngle = light.spotAngle;
        float lightRange = light.range;

        vec3 lightDir = normalize(lightPos - Position);
        float lightDistance = length(lightPos - Position);

        if (lightDistance > light.range)
            continue;

        float attenuation = clamp(1.0 - (lightDistance*lightDistance) / (lightRange*lightRange), 0.0, 1.0);
        vec3 lightPower = lightColor * lightIntensity * attenuation;

        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);
        // float energyConservation = (8.0 + Shininess) / (8.0 * 3.14159265);
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), Shininess) * Reflectivity;
        vec3 specular = spec * Specular * lightPower;

        // diffuse
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * lightPower;

        lighting += Albedo * diffuse + specular;
    }
    FragColor.rgb = lighting;
    
    FragColor.a = 1.0;
}
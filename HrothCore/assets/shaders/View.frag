#version 460 core

layout (location = 0) in vec2 uv;

layout(std140, binding = 0) uniform PerFrameData
{
    uniform mat4 iView;
    uniform mat4 iProj;
    uniform vec2 iResolution;
};

uniform sampler2D iTex1;
uniform sampler2D iTex2;
uniform sampler2D iTex3;
uniform sampler2D iTex4;

layout (location = 0) out vec4 FragColor;

void main()
{
    vec2 fragCoord = uv * iResolution;
    
    float gridThickness = 5.0;
    // Draw border around textures
    if (fragCoord.x < gridThickness || fragCoord.x > iResolution.x - gridThickness || fragCoord.y < gridThickness || fragCoord.y > iResolution.y - gridThickness
        || (fragCoord.x > iResolution.x / 2 - (gridThickness / 2) && fragCoord.x < iResolution.x / 2 + (gridThickness / 2))
        || (fragCoord.y > iResolution.y / 2 - (gridThickness / 2) && fragCoord.y < iResolution.y / 2 + (gridThickness / 2)))
    {
        FragColor = vec4(1.0, 0.7, 0.0, 1.0);
        return;
    }

    // Display albedo then normal then position then depth in every corner of the screen
    if (fragCoord.x < iResolution.x / 2 && fragCoord.y < iResolution.y / 2)
    {
        FragColor = vec4(texture(iTex1, vec2(uv.x * 2, uv.y * 2)).rgb, 1.0);
    }
    else if (fragCoord.x > iResolution.x / 2 && fragCoord.y < iResolution.y / 2)
    {
        FragColor = vec4(texture(iTex2, vec2((uv.x - 0.5) * 2, uv.y * 2)).rgb, 1.0);
    }
    else if (fragCoord.x < iResolution.x / 2 && fragCoord.y > iResolution.y / 2)
    {
        FragColor = vec4(texture(iTex3, vec2(uv.x * 2, (uv.y - 0.5) * 2)).rgb / 255.0, 1.0);
    }
    else if (fragCoord.x > iResolution.x / 2 && fragCoord.y > iResolution.y / 2)
    {
        FragColor = vec4(vec3(texture(iTex4, vec2((uv.x - 0.5) * 2, (uv.y - 0.5) * 2)).r), 1.0);
    }
}
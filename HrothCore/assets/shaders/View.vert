#version 460 core

layout (location = 0) out vec2 uv;

void main(void)
{
    vec2 vertices[3] = vec2[3](vec2(-1,-1), vec2(3,-1), vec2(-1, 3)); //Triangle taking up the whole screen

    gl_Position = vec4(vertices[gl_VertexID], 0 , 1);
    uv = 0.5 * gl_Position.xy + vec2(0.5);
}
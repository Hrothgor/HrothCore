#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textCoord;

layout(std140, binding = 0) uniform PerFrameData
{
	uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;
	uniform int isWireframe;
};

layout (location = 0) out vec3 color;

void main() {
    mat4 MVP = proj * view * model;
    gl_Position = MVP * vec4(pos, 1.0);
    color = isWireframe > 0 ? vec3(0.0) : pos;
}
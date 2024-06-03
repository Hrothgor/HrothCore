#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textCoord;

layout(std140, binding = 0) uniform PerFrameData
{
    uniform mat4 view;
    uniform mat4 proj;
};

struct PerMeshData_t
{
    mat4 model;
};

layout(std430, binding = 0) readonly buffer perMeshData {
    PerMeshData_t PerMeshData[];
};

const vec3 posT[8] = vec3[8](
	vec3(-1.0,-1.0, 1.0),
	vec3( 1.0,-1.0, 1.0),
	vec3( 1.0, 1.0, 1.0),
	vec3(-1.0, 1.0, 1.0),

	vec3(-1.0,-1.0,-1.0),
	vec3( 1.0,-1.0,-1.0),
	vec3( 1.0, 1.0,-1.0),
	vec3(-1.0, 1.0,-1.0)
);

const int indices[36] = int[36](
	// front
	0, 1, 2, 2, 3, 0,
	// right
	1, 5, 6, 6, 2, 1,
	// back
	7, 6, 5, 5, 4, 7,
	// left
	4, 0, 3, 3, 7, 4,
	// bottom
	4, 5, 1, 1, 0, 4,
	// top
	3, 2, 6, 6, 7, 3
);

layout (location = 0) out vec3 color;

void main() {
    mat4 MVP = proj * view * PerMeshData[gl_InstanceID].model;
    gl_Position = MVP * vec4(posT[indices[gl_VertexID]], 1.0);

    color = posT[indices[gl_VertexID]];
}
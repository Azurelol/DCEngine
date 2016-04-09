#version 330

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 TexCoords>

uniform mat4 model;

out vec3 WorldCoords;

void main()
{
	vec4 temp = model * vec4(vertex.xy, 0.0, 1.0);
	WorldCoords = vec3(temp.xyz);
}
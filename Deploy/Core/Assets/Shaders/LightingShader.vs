#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 TexCoords>

out vec2 gTexCoords;

void main(void)
{
	gl_Position = vec4(vertex.xy, 0.0, 1.0);
	gTexCoords = vertex.zw;
}
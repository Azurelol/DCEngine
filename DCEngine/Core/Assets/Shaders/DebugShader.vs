#version 330

layout (location = 0) in vec2 Vertex;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

void main(void)
{
	gl_Position = Proj * View * Model * vec4(Vertex, 0, 1);
}
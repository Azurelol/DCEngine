#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec4 color;
layout (location = 2) in mat4 model;

uniform mat4 view;
uniform mat4 projection;

out vec4 fragColor;

void main()
{
	fragColor = color;
	gl_Position = projection * view * model * vec4(vertex.xy, 0, 1);
}
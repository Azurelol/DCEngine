#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in mat4 model;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_position = projection * view * model * vec4(position.xy, 0, 1);
}
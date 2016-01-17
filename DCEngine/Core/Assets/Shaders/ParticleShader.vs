#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position.xy + offset, 0, 1);
}
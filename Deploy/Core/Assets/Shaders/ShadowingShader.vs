#version 330 core

layout (location = 0) in vec4 vertex; 

uniform mat4 model; 

out vec3 WorldPos;

void main() 
{ 
    vec4 temp = model * vec4(vertex.xy, 0.0, 1.0);
		WorldPos = temp.xyz;
}
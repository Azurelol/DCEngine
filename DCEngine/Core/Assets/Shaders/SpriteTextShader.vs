// Vertex shader used by 'SpriteText'
#version 330 core

layout (location = 0) in vec4 vertex; 

out vec3 WorldCoords;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{  
	vec4 temp = model * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
	WorldCoords = temp.xyz;
  gl_Position = projection * view * temp;
}
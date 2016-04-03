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
  WorldCoords = vec3(model * vec4(vertex.xy, 0,1));
  TexCoords = vertex.zw;
  gl_Position = projection * view * model * vec4(vertex.xy, 0, 1);
}
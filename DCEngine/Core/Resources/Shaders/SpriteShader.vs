#version 330 core

// We can store both position and texture-coordinate data in a single 'vec4' variable
// Because both position and texture coordinates contain two floats, we can combine them
// into a single vertex attribute. 
layout (location = 0) in vec4 vertex; 

out vec2 TexCoords; 

uniform mat4 model;
// By having an uniform color vetor we can easily change the color of sprites from
// gamecode
uniform mat4 projection;

void main() {
  TexCoords = vertex.zw;
  gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);

}
#version 330 core

// We can store both position and texture-coordinate data in a single 'vec4' variable
// Because both position and texture coordinates contain two floats, we can combine them
// into a single vertex attribute. 
layout (location = 0) in vec4 vertex; 
layout (location = 0) in vec3 position;

out vec2 TexCoords; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  TexCoords = vertex.zw;
  gl_Position =  model * vec4(vertex.xy, 0.0, 1.0);
  //gl_Position = vec4(vertex.xy, 0.0, 1.0);
  //gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);

}
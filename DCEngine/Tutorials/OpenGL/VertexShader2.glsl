#version 330 core

// The position variable has attribute position 0
layout (location = 0) in vec3 position; 
// The color variable has attribute position 1
layout (location = 1) in vec3 color;

// Output a color to the fragment shader
out vec3 ourColor;

uniform float xOffset;

void main(){
  //gl_Position = vec4(position, 1.0);
  // This will make the object flip from its y position
  // and add an uniform that will allow a vertex offset
  gl_Position = vec4(position.x + xOffset, -position.y, position.z, 1.0);
  // Set ourColor to the input color we get from the vertex data
  ourColor = color;
}
#version 330 core

/* The fragment shader only requires one output variable and that is a vector
   of size 4 that defines the color output that should be calculated. 
   We can declare output values with the 'out' keyword.
*/
 
 // The input variable from the vertex shader (same name, same type)
in vec4 vertexColor; 
out vec4 color; 

void main() {
  // We set it as the orange color with alpha 1
  color = vertexColor;;
}
#version 330 core

/* The fragment shader only requires one output variable and that is a vector
   of size 4 that defines the color output that should be calculated. 
   We can declare output values with the 'out' keyword.
*/
 
out vec4 color; 

void main() {
  // We set it as the orange color with alpha 1
  color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
#version 330 core

/* Uniforms are another way to pass data from our application on the CPU
   to the shaders on the GPU, but slightly different compared to vertex attributes.

   Uniforms are global, meaning that an uniform variable is unique per shader
   program object, and can be accessed from any shader at any stage in the shader
   program. Second, whatever you set the uniform value to, will stay at that value
   until it's reset or updated.
*/

in vec3 ourColor;

out vec4 color;

// To declare an uniform in OpenGL, simply add the 'uniform' keyword 
// This uniform is currently empty, it needs to have data added to it
// by finding its index/location in the update/drawing code.
// uniform vec4 ourColor;


void main() {
  // Since uniforms are global variables, we can define them in any shader
  // we'd like so there's no need to go through the vertex shader again.
  // color = ourColor;


  // Part 2: No longer using an uniform for the fragment's color
  color = vec4(ourColor, 1.0f);

}